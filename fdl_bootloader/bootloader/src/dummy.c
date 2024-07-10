int dummy (void);
extern void set_pc (void);

int dummy()
{
    set_pc();
    return 1;
}

int  SCI_GetAssertFlag (void)
{
    return 0;
}
