#include "globals.h"

bool moveLimiting = false;
int moveLimit = 0;

bool timeLimiting = false;
int timeLimit = 0;

bool fullSearch = false;

bool iterationLimiting = false;
int iterationLimit = 0;

int mode = MODE_CPHF;

bool outputState = true;
bool startOver = false;
bool startWithBest = false;

int tabuCount = 50000;