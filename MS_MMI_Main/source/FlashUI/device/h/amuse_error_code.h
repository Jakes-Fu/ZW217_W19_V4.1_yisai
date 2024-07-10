/*
 * amuse_error_code.h
 * Copyright (C) 2011 Microrapid Inc
 * Author: Tim <tim@Microrapid.com>
 * 
 * This file contains error codes definitions.
 * One of these codes will be returned to customer device program, 
 * if exception happened internally in Amuse.
 */


#ifndef AMUSE_ERROR_CODE_H
#define AMUSE_ERROR_CODE_H

/* Program exception happened. Assert failed. */
#define AMUSE_ASSERT_FATAL -1

/* Memory not enough in amuse. Can't continue to run. */
#define AMUSE_MEMORY_NOT_ENOUGH -2

/* Some flash functions not implemented by amuse yet. */
#define AMUSE_NOT_IMPLEMENT -3

/**
 * Swf version is higher than what Amuse can support. 
 * For example, if the loaded swf is using AS3, this exception will be triggered.
 */
#define AMUSE_NOT_SUPPORT -4

/* 
 * Return this error when the main movie swf or the swf to be loaded to the _level0 
 * fails to load. For example, swf file doesn't exist or is not a correct flash file.
 */
#define AMUSE_MAIN_MOVIE_LOAD_ERROR -5

#endif
