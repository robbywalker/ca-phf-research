extern bool moveLimiting;
extern int moveLimit;

extern bool timeLimiting;
extern int timeLimit; // in seconds

extern bool fullSearch;

extern bool iterationLimiting;
extern int iterationLimit;

extern bool outputState;

#ifndef MODE_CPHF

#define MODE_CPHF 1
#define MODE_PHF 2
#define MODE_WORD 3
#define MODE_SUM 4
#define MODE_VCPHF 5
#define MODE_PCA 6
#define MODE_CA 7
#define MODE_CPHFR 8
#define MODE_QCA 9
#define MODE_QCPHF 10
#define MODE_PHF_C1 11

#endif

extern int mode;
extern bool startOver;
extern bool startWithBest;

extern int tabuCount;