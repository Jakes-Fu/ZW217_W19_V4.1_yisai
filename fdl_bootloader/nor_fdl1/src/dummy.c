int dummy (void);
extern void JumpToTarget (void);

int dummy()
{
    JumpToTarget();
    return 1;
}
