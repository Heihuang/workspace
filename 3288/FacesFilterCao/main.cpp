#include "message/recvmessagemq.h"
#include <stdio.h>
int main(int argc, char **argv)
{
    printf("Welcome Entre FaceOS\n");
    printf("     -   _   -\n");
    RecvmessageMQ* recver = new RecvmessageMQ();
    if(recver == NULL)
    {
        return 0;
    }
    recver->CreateConsumer();
    return 0;
}
