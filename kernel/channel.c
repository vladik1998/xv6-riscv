#include "spinlock.h"
#include "channel.h"
#include "param.h"
#include "proc.h"
#include "defs.h"

struct channel channel[NCHANNELS];
int nextcid = 1;

void channelinit(void){
    struct channel *c;

    int index = 0;
    for(c = channel; c < &channel[NCHANNELS]; c++) {
        initlock(&c->lock, "channel");
        initlock(&c->consumer_lock, "consumer");
        initlock(&c->producer_lock, "producer");
        c->data = 0;
        c->state = 1;
        c->pid = -1;
        c->cid = index++;
    }
}
int channel_create(void){
    struct channel *c;
    struct proc *p = myproc();
    for(c = channel; c < &channel[NCHANNELS]; c++) {
        acquire(&c->lock);
        if(c->state) {
        goto found;
        } else {
        release(&c->lock);
        }
    }
    return 0;

found:
    c->state = 0;
    c->pid = p->pid;
    // channel was just created, it is empty so can't comsume yet.
    acquire(&c->consumer_lock);
    return c->cid;
    //return with c->lock held.
}

int channel_put(int cd, int data){
    if(cd < 0 || cd >= NCHANNELS)
        return -1;
    struct channel *c = &channel[cd];
    // check if the channel is in use.
    acquire(&c->lock);
    if(c->state){
        release(&c->lock);
        return -1;
    }
    release(&c->lock);
    // acquire a lock to insert data into the channel.
    acquire(&c->producer_lock);
    // acquire a lock to the channel itself.
    acquire(&c->lock);
    if(c->state){
        // release all locks if needed.
        release(&c->lock);
        release(&c->consumer_lock);
        release(&c->producer_lock);
        return -1;
    }
    c->data = data;
    release(&c->lock);
    // release the lock to the channel itself.
    release(&c->consumer_lock);
    // release the lock to consumers that want to take the data from the channel.
    return 0;
}

int channel_take(int cd, int* data){
    if(cd < 0 || cd >= NCHANNELS)
        return -1;
    struct channel *c = &channel[cd];
    // check if the channel is in use.
    acquire(&c->lock);
    if(c->state){
        release(&c->lock);
        return -1;
    }
    release(&c->lock);
    // acquire a lock to take data from the channel.
    acquire(&c->consumer_lock);
    // acquire a lock to the channel itself.
    acquire(&c->lock);
    if(c->state){
        // release all locks if needed.
        release(&c->lock);
        release(&c->producer_lock);
        release(&c->consumer_lock);
        return -1;
    }
    struct proc *p = myproc();
    // copy the data from the channel to the process user space.
    copyout(p->pagetable, data, (char *)&(c->data), sizeof(c->data));
    release(&c->lock);
    // release the lock to the channel itself.
    release(&c->producer_lock);
    // release the lock to producers that want to insert data into the channel.
    return 0;
}

int channel_destroy(int cd){
    if(cd < 0 || cd >= NCHANNELS)
        return -1;
    struct channel *c = &channel[cd];
    acquire(&c->lock);
    if(c->state){
        release(&c->lock);
        return -1;
    }
    // now it is Unused
    c->state = 1;
    release(&c->producer_lock);
    release(&c->consumer_lock);
    release(&c->lock);
    return 0;
}


