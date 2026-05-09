/*
 * This  file  is  part  of  the  Cadence  LEF/DEF  Open   Source
 * Distribution,  Product Version 5.7, and is subject to the Cadence LEF/DEF
 * Open Source License Agreement.   Your  continued  use  of this file
 * constitutes your acceptance of the terms of the LEF/DEF Open Source
 * License and an agreement to abide by its  terms.   If you  don't  agree
 * with  this, you must remove this and any other files which are part of the
 * distribution and  destroy any  copies made.
 * 
 * For updates, support, or to become part of the LEF/DEF Community, check
 * www.openeda.org for details.
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lex.h"
#include "defiDebug.h"
#include "defiMisc.h"

/*
 * Handle points for a polygon
 */ defiGeometries *
defiGeometries_CreateUninitialized()
{
  defiGeometries *this = (defiGeometries *) malloc(sizeof(defiGeometries));

  defiGeometries_Init(this);
  return (this);
}

void 
defiGeometries_Init(defiGeometries * this)
{
  this->pointsAllocated_ = 0;
  this->numPoints_ = 0;
}

void 
defiGeometries_Destroy(defiGeometries * this)
{
  if (this->pointsAllocated_) {
    defFree((char *) (this->x_));
    defFree((char *) (this->y_));
  }
  this->pointsAllocated_ = 0;
  this->numPoints_ = 0;
}

void
defiGeometries_Delete(defiGeometries * this)
{
  if (this == NULL)
    return;
  defiGeometries_Destroy(this);
  free(this);
}

void 
defiGeometries_Reset(defiGeometries * this)
{
  this->numPoints_ = 0;
}

void 
defiGeometries_startList(defiGeometries * this,
			 int x,
			 int y)
{
  if (this->pointsAllocated_ == 0) {
    this->pointsAllocated_ = 16;
    this->x_ = (int *) defMalloc(sizeof(int) * 16);
    this->y_ = (int *) defMalloc(sizeof(int) * 16);
    this->numPoints_ = 0;
  }
  else
    this->numPoints_ = 0;
  defiGeometries_addToList(this,
			   x,
			   y);
}

void 
defiGeometries_addToList(defiGeometries * this,
			 int x,
			 int y)
{
  if (this->numPoints_ == this->pointsAllocated_) {
    int     i;

    int    *nx;

    int    *ny;

    this->pointsAllocated_ *= 2;
    nx = (int *) defMalloc(sizeof(int) * this->pointsAllocated_);
    ny = (int *) defMalloc(sizeof(int) * this->pointsAllocated_);
    for (i = 0;
	 i < this->numPoints_;
	 i++) {
      nx[i] = this->x_[i];
      ny[i] = this->y_[i];
    }
    defFree((char *) (this->x_));
    defFree((char *) (this->y_));
    this->x_ = nx;
    this->y_ = ny;
  }
  this->x_[this->numPoints_] = x;
  this->y_[this->numPoints_] = y;
  this->numPoints_ += 1;
}

int 
defiGeometries_numPoints(defiGeometries * this)
{
  return this->numPoints_;
}

void 
defiGeometries_points(defiGeometries * this,
		      int index,
		      int *x,
		      int *y)
{
  char    msg[160];

  if ((index < 0) || (index >= this->numPoints_)) {
    sprintf(msg, "ERROR (LEFPARS-6070): The index number %d given for GEOMETRY POINTS is invalid.\nValid index is from 0 to %d", index, this->numPoints_);
    defiError(msg);
    return;
  }
  *x = this->x_[index];
  *y = this->y_[index];
  return;
}

/*
 * defiStyles
 */ defiStyles *
defiStyles_CreateUninitialized()
{
  defiStyles *this = (defiStyles *) malloc(sizeof(defiStyles));

  defiStyles_Init(this);
  return (this);
}

void 
defiStyles_Init(defiStyles * this)
{
  this->styleNum_ = 0;
  this->polygon_ = 0;
}

void
defiStyles_Delete(defiStyles * this)
{
  if (this == NULL)
    return;
  defiStyles_Destroy(this);
  free(this);
}

void 
defiStyles_Destroy(defiStyles * this)
{
  defiStyles_clear(this);
}

void 
defiStyles_clear(defiStyles * this)
{
  struct defiPoints *p;

  p = this->polygon_;
  if (p) {
    defFree((char *) (p->x));
    defFree((char *) (p->y));
    defFree((char *) (this->polygon_));
  }
  this->styleNum_ = 0;
  this->polygon_ = 0;
}

void 
defiStyles_setStyle(defiStyles * this,
		    int styleNum)
{
  this->styleNum_ = styleNum;
}

void 
defiStyles_setPolygon(defiStyles * this,
		      defiGeometries * geom)
{
  struct defiPoints *p;

  int     i, x, y;

  if (this->polygon_ == 0) {
    p = (struct defiPoints *) defMalloc(sizeof(struct defiPoints));
    p->numPoints = defiGeometries_numPoints(geom);
    p->x = (int *) defMalloc(sizeof(int) * p->numPoints);
    p->y = (int *) defMalloc(sizeof(int) * p->numPoints);
    this->numPointAlloc_ = p->numPoints;
  }
  else if (this->numPointAlloc_ < defiGeometries_numPoints(geom)) {
    p = this->polygon_;
    defFree((char *) (p->x));
    defFree((char *) (p->y));
    p->numPoints = defiGeometries_numPoints(geom);
    p->x = (int *) defMalloc(sizeof(int) * p->numPoints);
    p->y = (int *) defMalloc(sizeof(int) * p->numPoints);
    this->numPointAlloc_ = p->numPoints;
  }
  else {
    p = this->polygon_;
    p->numPoints = defiGeometries_numPoints(geom);
  }
  for (i = 0;
       i < p->numPoints;
       i++) {
    defiGeometries_points(geom,
			  i,
			  &x,
			  &y);
    p->x[i] = x;
    p->y[i] = y;
  }
  this->polygon_ = p;
}

int 
defiStyles_style(const defiStyles * this)
{
  return this->styleNum_;
}

struct defiPoints 
defiStyles_getPolygon(const defiStyles * this)
{
  return *(this->polygon_);
}
