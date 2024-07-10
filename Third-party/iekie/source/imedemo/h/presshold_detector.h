
/*
 * presshold_detector.h
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#ifndef PRESSHOLD_DETECTOR_H
#define PRESSHOLD_DETECTOR_H

#define GUI_MSG_KEYPRESSHOLD    4

#define GUI_PRESSHOLD_TIMEOUT   1300

typedef struct tagPRESSHOLD_DETECTOR
{
    int     started;
    int     ignore;
    int     start_time;
    int     key;
} PRESSHOLD_DETECTOR;

void presshold_detector_reset(
    PRESSHOLD_DETECTOR* detector);

int presshold_detector_check(
    PRESSHOLD_DETECTOR* detector,
    int msg, int key, int time);

#endif /* PRESSHOLD_DETECTOR_H */
