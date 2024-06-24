// Per-process state
struct channel {
    struct spinlock lock;
    struct spinlock consumer_lock;
    struct spinlock producer_lock;
    int data; // Data in the channel
    int pid; // Process ID of the process that created the channel
    int state; // Status of the channel (Used - 0, Unused - 1)
    int cid; // Channel ID
};

