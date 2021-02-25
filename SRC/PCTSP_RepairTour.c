#include "LKH.h"
#include "Heap.h"
#include "Segment.h"

#define InDegree V

GainType PCTSP_RepairTour()
{
    GainType P = Penalty(); int Min = INT_MAX;
    Node* N1 = Depot;
    while ((N1 = SUCC(N1))->DepotId == 0);
    Node* N2 = Depot;
    while ((N2 = PREDD(N2))->DepotId == 0);
    int Forward = N1 != N2 ? N1->DepotId < N2->DepotId : !Reversed;
    

    {
        for (int i = 1; i <= DimensionSaved; ++i) {
            Node* N = &NodeSet[i];
            if (N->DepotId != 0) {
                continue;
            }
            if(N->Colors[N->Color] != 1){
                // choose color
                int _color;
                do
                {
                    _color = Random() % Salesmen + 1;
                } while (N->Colors[_color] == 0);

                Node* new_depot = _color == 1 ? Depot :  &NodeSet[Dim + _color - 1];
                //Follow(&NodeSet[N->Id + DimensionSaved], new_depot);
                if (Forward)
                {
                    Follow(N, new_depot);
                }         
                else
                    Precede(N, new_depot);          
                N->Color = _color;
            }
        }
    }

    Node* N, * CurDepot, * NextN;
    GainType Cost;
    int Set, FringeNodes, Count, i;
    Constraint* Con;


    Node** Fringe, * First = 0, * Last;
    Fringe = (Node**)malloc(DimensionSaved * sizeof(Node*));


    First = Last = Depot;
    First->Prev = First->Next = First;

    N = First;
    do
        N->InDegree = 0;
    while ((N = N->Suc) != First);

    N = CurDepot = Depot;
    do
    {
        CurDepot = N;
        do {
            if (N->Id <= DimensionSaved)
            {
                for (Con = N->FirstConstraint; Con; Con = Con->Next)
                {
                    if (Con->t2->Color == CurDepot->DepotId)
                    {
                        Con->t2->InDegree++;
                    }
                }
            }
        } while ((N = Forward ? SUCC(N) : PREDD(N))->DepotId == 0);
    } while (N != Depot);

    N = CurDepot = Depot;
    do {
        CurDepot = N;
        FringeNodes = 0;
        memset(Fringe, 0, DimensionSaved * sizeof(Node*));
        do
        {
            if (N->Id <= DimensionSaved && N->InDegree == 0 && N != CurDepot) {
                Fringe[FringeNodes++] = N;
            }
        } while ((N = Forward ? SUCC(N) : PREDD(N))->DepotId == 0);
       
        Node* last = N;
        Node* prev = CurDepot;
        while (FringeNodes > 0) {
            Count = 0;
            Min = prev->C[Fringe[0]->Id];
            i = 0;
            for (int j = 1; j < FringeNodes; j++)
            {
                if (prev->C[Fringe[j]->Id] < Min)
                {
                    Min = prev->C[Fringe[j]->Id];
                    i = j;
                }
            }      
            //i = Random() % FringeNodes;
            assert(Fringe[i]->Color == CurDepot->DepotId);
            if (Forward) {
                Follow(Fringe[i] + DimensionSaved, prev);
                Follow(Fringe[i], Fringe[i] + DimensionSaved);
            }
            else {
                Precede(Fringe[i] + DimensionSaved, prev);
                Precede(Fringe[i], Fringe[i] + DimensionSaved);
            }
            prev = Fringe[i];
            Fringe[i] = Fringe[--FringeNodes];

            for (Con = prev->FirstConstraint; Con; Con = Con->Next) {
                if (Con->t2->Color == CurDepot->DepotId)
                {
                    if (--Con->t2->InDegree == 0)
                        Fringe[FringeNodes++] = Con->t2;
                    else if (Con->t2->InDegree < 0)
                        eprintf("SOP_InitialTour: Precedence cycle detected");
                }
            }
        }
        N = last;
    } while (N != Depot);

    free(Fringe);

    Cost = 0;
    do
        Cost += C(N, N->Suc) - N->Pi - N->Suc->Pi;
    while ((N = N->Suc) != First);
    CurrentPenalty = 0; 
    return Cost / Precision;
}
