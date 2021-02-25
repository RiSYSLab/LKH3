#include "LKH.h"
#include "Segment.h"

/*
 * The RecordBetterTour function is called by FindTour each time
 * the LinKernighan function has returned a better tour.
 *
 * The function records the tour in the BetterTour array and in the
 * BestSuc field of each node. Furthermore, for each node the previous
 * value of BestSuc is saved in the NextBestSuc field.
 *
 * Recording a better tour in the BetterTour array when the problem is
 * asymmetric requires special treatment since the number of nodes has
 * been doubled.
 */

void RecordBetterTour()
{


    if (ProblemType == PCTSP) {
        static Node* StartRoute = 0;
        Node* N, * N1, * N2, * CurrentRoute;
        int Forward;
        N1 = Depot;
        while ((N1 = SUCC(N1))->DepotId == 0);
        N2 = Depot;
        while ((N2 = PREDD(N2))->DepotId == 0);
        Forward = N1 != N2 ? N1->DepotId < N2->DepotId : !Reversed;
        if (!StartRoute)
            StartRoute = Depot;
        N = StartRoute;
        int i = 1;
        do {
            CurrentRoute = N;
            Node* Prev = NULL;
            do {
                if (N->Id <= DimensionSaved)
                {
                    BetterTour[i++] = N->Id;
                }
                N->NextBestSuc = N->BestSuc;
                N->BestSuc = Forward ? SUCC(N) : PREDD(N);
            } while ((N = Forward ? SUCC(N) : PREDD(N))->DepotId == 0);
        } while (N != StartRoute);
        BetterTour[0] = BetterTour[DimensionSaved];
    }
    else {
        Node* N = Asymmetric ? Depot : FirstNode;
        Node* Stop = N;

        if (!Asymmetric) {
            int i = 1;
            do
                BetterTour[i++] = N->Id;
            while ((N = N->Suc) != Stop);
        }
        else if (N->Suc->Id != DimensionSaved + N->Id) {
            int i = 1;
            do
                if (N->Id <= DimensionSaved)
                    BetterTour[i++] = N->Id;
            while ((N = N->Suc) != Stop);
        }
        else {
            int i = DimensionSaved;
            do
                if (N->Id <= DimensionSaved)
                    BetterTour[i--] = N->Id;
            while ((N = N->Suc) != Stop);
        }
        BetterTour[0] = BetterTour[DimensionSaved];
        do {
            N->NextBestSuc = N->BestSuc;
            N->BestSuc = N->Suc;
        } while ((N = N->Suc) != Stop);
    }
}
