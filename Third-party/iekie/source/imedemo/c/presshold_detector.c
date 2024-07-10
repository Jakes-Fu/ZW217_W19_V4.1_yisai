
/*
 * presshold_detector.c
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#include "presshold_detector.h"
#include "gui.h"

void presshold_detector_reset(
    PRESSHOLD_DETECTOR* detector)
{
    detector->started = FALSE;
}

int presshold_detector_check(
    PRESSHOLD_DETECTOR* detector,
    int msg, int key, int time)
{
    int result;
    int time_out;

    result = 0;
    
    if (detector->started && (key != detector->key))
        detector->started = FALSE;              /* Reset when key not match previous */

    if (detector->started)
    {
        if (detector->ignore)
        {
            if (msg == GUI_MSG_KEYUP)
                detector->started = FALSE;      /* Reset when key released */
        }
        else
        {
            time_out = (time - detector->start_time >= GUI_PRESSHOLD_TIMEOUT) ? 1 : 0;

            if (msg == GUI_MSG_KEYUP)
            {
                /* Generate key event */
                result = time_out ? GUI_MSG_KEYPRESSHOLD : GUI_MSG_KEYDOWN;
                detector->started = FALSE;      /* Reset when key released */
            }
            else
            {
                if (time_out)
                {
                    /* Generate presshold event */
                    result = GUI_MSG_KEYPRESSHOLD;
                    detector->ignore = TRUE;
                }
                else
                {
                    /* Continue checking, no event generated */
                }
            }
        }
    }
    else
    {
        if (msg == GUI_MSG_KEYUP)
        {
            /* Ignore when no match keydown */
        }
        else
        {
            /* Start checking */
            detector->started = TRUE;
            detector->ignore = FALSE;
            detector->start_time = time;
            detector->key = key;
        }
    }
    
    return result;
}
