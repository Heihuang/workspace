#include <map>
#include <vector>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include "servicemodules/featurecmpncao.h"
#include "servicemodules/featurecmp.h"
#include "servicemodules/managdb.h"
#include "message/recvmessagemq.h"
#include "message/sendmessageMQ.h"

int main(int argc, char **argv)
{
    Managdb *db = new Managdb();
    if(!db)
    {
        printf("new db failed\n");
        return -1;
    }
    RecvmessageMQ *recver = new RecvmessageMQ();
    if(!recver)
    {
        printf("new recver failed\n");
        return -1;
    }
    ListFeatureUpdate* update = new ListFeatureUpdate();
    if(!update)
    {
        printf("new update failed\n");
        return -1;
    }
    FeatureCmp *feacmp = NULL;
    feacmp = new FeatureCmpNCao();
    if(!feacmp)
    {
        printf("new feacmp failed\n");
        return -1;
    }

    recver->setFeatureCmp(feacmp);
    db->setFeatureCmp(feacmp);
    update->setFeatureCmp(feacmp);
    recver->CreateConsumer();
    recver->start();
    db->start();
    update->CreateConsumer();
    update->start();
    while (1)
    {
       sleep(5);
    }
    return 0;
}
