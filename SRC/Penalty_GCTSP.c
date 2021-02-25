#include "LKH.h"
#include "Segment.h"

GainType Penalty_GCTSP(void)
{
    static Node* StartRoute = 0;
    Node* N, * N1, * N2, * CurrentRoute;
    GainType P = 0;
    int Forward;
    int i = 1;
    N1 = Depot;
    while ((N1 = SUCC(N1))->DepotId == 0);
    N2 = Depot;
    while ((N2 = PREDD(N2))->DepotId == 0);
    Forward = N1 != N2 ? N1->DepotId < N2->DepotId : !Reversed;
    if (!StartRoute)
        StartRoute = Depot;
    N = StartRoute;
    do {
        CurrentRoute = N;
        do {
            if(N->Id <= DimensionSaved){
                // set color and Loc
                N->Loc = i++;
                N->Color = CurrentRoute->DepotId;
                if(N->Colors[N->Color] == 0){
                    P++;
                }
            }
            else {
                if (NodeSet[N->Id - DimensionSaved].DepotId != 0) {
                    Node* Next = Forward ? SUCC(N) : PREDD(N);                     
                    if (Next->DepotId == 0) {                         
                        //printf("Depot duplicate next node is not depot!\n");     
                        P++;                     
                    }                
                }             
            }
        } while ((N = Forward ? SUCC(N) : PREDD(N))->DepotId == 0);
    } while (N != StartRoute);
    return P;
}
