[1mdiff --git a/Solver/Algorithm/beamsearch.cpp b/Solver/Algorithm/beamsearch.cpp[m
[1mindex 77dc622..17f3df4 100644[m
[1m--- a/Solver/Algorithm/beamsearch.cpp[m
[1m+++ b/Solver/Algorithm/beamsearch.cpp[m
[36m@@ -19,7 +19,7 @@[m [mvoid BeamSearch::initialization()[m
 {[m
     cpu_num = std::thread::hardware_concurrency();[m
 #ifndef NO_PARALLEL[m
[31m-    beam_width = 100;[m
[32m+[m[32m    beam_width = 300;[m
 #else[m
     beam_width = 100;[m
 #endif[m
[36m@@ -242,17 +242,12 @@[m [mvoid BeamSearch::run(procon::Field field)[m
         this->evaluateNextMove(evaluations,field_vec);[m
         this->evaluateHistoryInit(field_vec);[m
         for(Evaluation & evaluation: evaluations) {[m
[31m-            std::cout << "alpha" << std::endl;[m
             evaluation.evaluation += alpha * this->evaluateUniqueAngle(evaluation,field_vec);[m
[31m-            std::cout << "beta" << std::endl;[m
             evaluation.evaluation += beta * this->evaluateUniqueLength(evaluation,field_vec);[m
[31m-            std::cout << "gamma" << std::endl;[m
             evaluation.evaluation += gamma * this->evaluateHistory(evaluation,field_vec);[m
[31m-            std::cout << "delta" << std::endl;[m
             evaluation.evaluation += delta * this->evaluateFrame(evaluation,field_vec);[m
         }[m
 [m
[31m-        std::cout << "clear" << std::endl;[m
         if (evaluations.empty()){[m
             submitAnswer(buckup_field);[m
             return;[m
[1mdiff --git a/Solver/Algorithm/beamsearch.h b/Solver/Algorithm/beamsearch.h[m
[1mindex 478638e..4e15f7a 100644[m
[1m--- a/Solver/Algorithm/beamsearch.h[m
[1m+++ b/Solver/Algorithm/beamsearch.h[m
[36m@@ -24,9 +24,9 @@[m [mpublic:[m
     void run(procon::Field field);[m
 [m
     double alpha = 2;[m
[31m-    double beta = 0.5;[m
[31m-    double gamma = 1;[m
[31m-    double delta = 3;[m
[32m+[m[32m    double beta = 2;[m
[32m+[m[32m    double gamma = 2;[m
[32m+[m[32m    double delta = 2;[m
 };[m
 [m
 [m
