#include "Debug.h"
#include "LKH.h"
#include "Segment.h"

void printPCTSPRoute()
{
    static Node* StartRoute = 0;
    Node* N, * N1, * N2, * CurrentRoute;
    int Forward;
    GainType Cost = 0;
    N1 = Depot;
    while ((N1 = SUCC(N1))->DepotId == 0);
    N2 = Depot;
    while ((N2 = PREDD(N2))->DepotId == 0);
    Forward = N1 != N2 ? N1->DepotId < N2->DepotId : !Reversed;
    if (!StartRoute)
        StartRoute = Depot;
    N = StartRoute;
    printf("Print Route: \t");
    do {
        CurrentRoute = N;
        Node* Prev = NULL;
        do {
            if (N->Id <= DimensionSaved && N->Colors[CurrentRoute->DepotId] == 0)
            {
                printf("break color");
            }
            if(N->Id <= DimensionSaved)
            {
               
                if(Prev){
                    Cost += Prev->C[N->Id];
                    printf(" %d",Prev->C[N->Id]);
                }
                Prev = N;
            }
            printf("[%d] -> " , N->Id);
        } while ((N = Forward ? SUCC(N) : PREDD(N))->DepotId == 0);
        printf("\n");
    } while (N != StartRoute);
   
    printf("Cost is %d\n", Cost);

    Cost = 0;
    N = Depot;
    do
    {
        Cost += C(N, SUCC(N)) - N->Pi - SUCC(N)->Pi;
        printf(" %d", C(N, SUCC(N)) - N->Pi - SUCC(N)->Pi);
    } while ((N = SUCC(N)) != Depot);
    Cost /= Precision;

    printf("Cost is %d\n", Cost);
    printf("printConstraints:\n");

    Constraint *Con, *ConPred, *ConSuc;
    for(Con = FirstConstraint; Con;
        ConPred = Con, Con = ConSuc)
    {
        ConSuc = Con->Suc;
        if(Con->t1->Color == Con->t2->Color &&
            Con->t1->Loc > Con->t2->Loc)
            {
                printf("break constraint [%d]->[%d]\n",Con->t1->Id,Con->t2->Id);
            }
    }
    printf("\n\n");
}

void printConstraints(){
    printf("printConstraints:\n");
    Constraint *Con, *ConPred, *ConSuc;
    int* hash_set = calloc(Dimension, sizeof(int));
    for(Con = FirstConstraint; Con;
        ConPred = Con, Con = ConSuc)
    {
        ConSuc = Con->Suc;
        if(hash_set[Con->t1->Id]){
            continue;
        }else{
            hash_set[Con->t1->Id] = 1;
        }
        Constraint* tmp = Con;
        do
        {
            printf("%d->%d\t",tmp->t1->Id, tmp->t2->Id);
        }while (tmp = tmp->Next);
        printf("\n");
    }
    printf("---------------------------\n\n");
    free(hash_set);
}

void printNodeSet(void(*func)(const Node*))
{
    Node* N = FirstNode;
    do
    {
        func(N);
    } while ((N = N->Suc) != FirstNode);
}

void printCandidateSet(const Node* node)
{
    if (node) {
        if (node->CandidateSet) {
            printf("Node %d CandidateSet: To ", node->Id);
            for (size_t i = 0; i < MaxCandidates; ++i) {
                if (node->CandidateSet[i].To) {
                    printf("%d ", node->CandidateSet[i].To->Id);
                }
                else {
                    break;
                }
            }
            printf("\n");
        }
        else {
            printf("Error: printCandidateSet : node->CandidateSet is NULL\n");
        }
    }
    else {
        printf("Error: printCandidateSet : arg node can't be NULL\n");
    }
}