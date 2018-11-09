/**
  ******************************************************************************
  * @file    opentherm_json.c
  * @author  turchenkov@gmail.com
  * @version V0.0.1
  * @date    07-Nov-2018
  * @brief   This file contains opentherm to json converter functions
  ******************************************************************************
  */

#include "opentherm_json.h"

static	const	uint8_t topic_json0[] = {"{\"Id\":\""};
#define	SIZ0 sizeof(topic_json0)

static	const	uint8_t topic_json1[] = {"\", \"Val\":\""};
#define	SIZ2 sizeof(topic_json1)

static	const	uint8_t topic_json2[] = {"\", \"Qual\":\""};
#define	SIZ4 sizeof(topic_json2)

static	const	uint8_t topic_json3[] = {"\", \"Ts\":\""};
#define	SIZ6 sizeof(topic_json3)

static	const	uint8_t	offtext[] = {"off"};
static	const	uint8_t	ontext[] = {" on"};

static	const	uint8_t	qgood[] = {"good"};
static	const 	uint8_t	qngood[] = {" bad"};
#define	SIZ5 sizeof(qngood)

static	uint8_t	ldids[] = {"$$$"};
#define	SIZ1 sizeof(ldids)

static	uint8_t	szival[] = {"-9876543210"};
#define	SIZ3 sizeof(szival)

static	uint8_t	timest[] = {"xxxxxxxxxx."};
#define	SIZ7 sizeof(timest)

static	uint8_t	timestms[] = {"xxx\"}"};
#define	SIZ8 sizeof(timestms)


static uint8_t		outstring[SIZ0+SIZ1+SIZ2+SIZ3+SIZ4+SIZ5+SIZ6+SIZ7+SIZ8];




