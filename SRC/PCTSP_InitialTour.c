#include "LKH.h"

/* The PCTSP_InitialTour function computes an initial tour for a General
 * colored TSP.
 */
#define InDegree V

GainType PCTSP_InitialTour()
{
    Node* N, *temp, *NextN;
    GainType Cost;
    int Set, FringeNodes, Count, i;
    Constraint* Con;
    double EntryTime = GetTime();

    Node** Fringe, * First = 0, * Last;
    int* Subset;
    Fringe = (Node**)malloc(DimensionSaved * sizeof(Node*));
    Subset = (int*)malloc(DimensionSaved * sizeof(int));
    
    //if (TraceLevel >= 1)
    //    printff("PCTSP = ");
    assert(Asymmetric);
    for (Set = 2; Set <= Salesmen; Set++)
        Follow(&NodeSet[Dim + Set - 1],
            Set == 2 ? Depot : &NodeSet[Dim + Set - 2]);
    
    //先按颜色分城市：
    N = Depot;
    do { 
        NextN = N->Suc;
        if (N->DepotId == 0) {
            while (1)
            {
                int salesman_index = Random() % Salesmen + 1;
                if (N->Colors[salesman_index] == 1) {
                    Set = salesman_index;
                    N->Color = Set;
                    break;
                }
            }
            Follow(N, Set == 1 ? Depot : &NodeSet[Dim + Set - 1]);
        }
    } while ((N = NextN) != Depot); 

    First = Last = Depot;
    First->Prev = First->Next = First;

    //再按约束排顺序：
    for (int j = 1; j <= Salesmen; j++)
    {
        if (j == 1)
            temp = Depot;
        else
            temp = &NodeSet[Dim + j - 1];

        N = temp;
        do
        {
            N = N->Suc;
            N->InDegree = 0;
        }          
        while (N->Suc->DepotId == 0);

        N = temp;
        do {      
            N = N->Suc;
            if (N->Id <= DimensionSaved)
            {
                for (Con = N->FirstConstraint; Con; Con = Con->Next)
                {
                    if (Con->t2->Color == temp->DepotId)
                    {
                        Con->t2->InDegree++;
                    }
                }
            }        
        } while (N->Suc->DepotId == 0);

        FringeNodes = 0;
        memset(Fringe, 0, DimensionSaved * sizeof(Node*));
        memset(Subset, 0, DimensionSaved * sizeof(int));
        
        N = temp;
        N->Prev = Last;
        N->Next = First;
        First->Prev = Last->Next = N;
        Last = N;

        do {
            N = N->Suc;
            if (N->Id <= DimensionSaved && N->InDegree == 0)
            {
                Fringe[FringeNodes++] = N;    
            }
        } while (N->Suc->DepotId == 0);

        N = temp;
        while (FringeNodes > 0) {
            Count = 0;
            for (i = 0; i < FringeNodes; i++)
                if (IsCandidate(N, Fringe[i] + DimensionSaved))
                    Subset[Count++] = i;
            i = Count > 0 ? Subset[Random() % Count] : Random() % FringeNodes; //候选集里面有的就在候选集里面选；                      
            Follow(Fringe[i] + DimensionSaved, N);
            Follow(Fringe[i], Fringe[i] + DimensionSaved);
            N = Fringe[i];
            Fringe[i] = Fringe[--FringeNodes];        
            N->Prev = Last;
            N->Next = First;
            First->Prev = Last->Next = N;
            Last = N;
            for (Con = N->FirstConstraint; Con; Con = Con->Next) {
                if (Con->t2->Color == temp->DepotId)
                {
                    if (--Con->t2->InDegree == 0)
                        Fringe[FringeNodes++] = Con->t2;
                    else if (Con->t2->InDegree < 0)
                        eprintf("SOP_InitialTour: Precedence cycle detected");
                }               
            }
        }
    }
    free(Fringe);
    free(Subset);

    Cost = 0;
    N = FirstNode;
    do
    {
        Cost += C(N, N->Suc) - N->Pi - N->Suc->Pi;
    } while ((N = N->Suc) != FirstNode);
    Cost /= Precision;

    CurrentPenalty = PLUS_INFINITY;
    CurrentPenalty = Penalty();


    /*if (TraceLevel >= 1) {
        if (Salesmen > 1 || ProblemType == SOP)
            printff(GainFormat "_" GainFormat, CurrentPenalty, Cost);
        else
            printff(GainFormat, Cost);
        if (Optimum != MINUS_INFINITY && Optimum != 0)
            printff(", Gap = %0.2f%%", 100.0 * (Cost - Optimum) / Optimum);
        printff(", Time = %0.2f sec.\n", fabs(GetTime() - EntryTime));
    }*/
    return Cost;
}
