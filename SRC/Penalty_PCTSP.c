#include "LKH.h"
#include "Segment.h"

GainType Penalty_PCTSP()
{

    GainType P_GCTSP = Penalty_GCTSP();
    if (P_GCTSP > CurrentPenalty) {
       return P_GCTSP;
    }
    GainType P_SOP = Penalty_PCTSP_SOP();
     // printf("P_GCTSP is %lld, P_SOP is %lld\n", P_GCTSP, P_SOP);
    // if(P_GCTSP + P_SOP == 0){
    //     printPCTSPRoute();
    // }
    // if (P_SOP == 0)
    //    printPCTSPRoute();
    return P_GCTSP + P_SOP;
}

GainType Penalty_PCTSP_SOP(void)
{
    Node* First = &NodeSet[1], * N = First;
    GainType P = 0, i = 1, j;
    Constraint* ConPred = 0, * ConSuc = 0;
    static Constraint* Con = 0;
    
    if (CurrentPenalty == 0) {
        if (Con && Con->t1->Loc > Con->t2->Loc
            && Con->t1->Color == Con->t2->Color)
            return 1;
        for (i = Swaps - 1; i >= 0; i--) {
            for (j = 1; j <= 4; j++) {
                N = j == 1 ? SwapStack[i].t1 :
                    j == 2 ? SwapStack[i].t2 :
                    j == 3 ? SwapStack[i].t3 : SwapStack[i].t4;
                if (N->Id <= DimensionSaved) {
                    for (Con = N->FirstConstraint; Con; Con = Con->Next)
                        if (Con->t1->Loc > Con->t2->Loc 
                             && Con->t1->Color == Con->t2->Color)
                            return 1;
                }
            }
        }
    }
    
    for (Con = FirstConstraint; Con; ConPred = Con, Con = ConSuc) {
        ConSuc = Con->Suc;
        if (Con->t1->Loc > Con->t2->Loc && Con->t1->Color == Con->t2->Color) {
            if (Con != FirstConstraint) {
                ConPred->Suc = ConSuc;
                Con->Suc = FirstConstraint;
                FirstConstraint = Con;
                Con = ConPred;
            }
            if (++P > CurrentPenalty ||
                (P == CurrentPenalty && CurrentGain <= 0))
                return CurrentPenalty + (CurrentGain > 0);
        }
    }
    return P;
}
