// AUTO GENERATED

void RPC_rpcHelloOnAP(void *in, void *out)
{
    struct IPAR_rpcHelloOnAP *ipar = (struct IPAR_rpcHelloOnAP *)in;
    struct OPAR_rpcHelloOnAP *opar = (struct OPAR_rpcHelloOnAP *)out;
    opar->result = rpcHelloOnAP(ipar->n);
}


// AUTO GENERATED END
