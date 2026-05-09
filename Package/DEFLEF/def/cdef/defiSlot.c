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
#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "defiSlot.h"
#include "defiDebug.h"

/*
 * defiSlot
 */ defiSlot *
defiSlot_CreateUninitialized()
{
  defiSlot *this = (defiSlot *) malloc(sizeof(defiSlot));

  defiSlot_Init(this);
  return (this);
}

void 
defiSlot_Init(defiSlot * this)
{
  this->numPolys_ = 0;
  defiSlot_clear(this);
  this->layerNameLength_ = 0;
  this->xl_ = (int *) defMalloc(sizeof(int) * 1);
  this->yl_ = (int *) defMalloc(sizeof(int) * 1);
  this->xh_ = (int *) defMalloc(sizeof(int) * 1);
  this->yh_ = (int *) defMalloc(sizeof(int) * 1);
  this->rectsAllocated_ = 1;
  this->polysAllocated_ = 0;
  this->polygons_ = 0;
  this->layerName_ = 0;
}

void
defiSlot_Delete(defiSlot * this)
{
  if (this == NULL)
    return;
  defiSlot_Destroy(this);
  free(this);
}

void 
defiSlot_clear(defiSlot * this)
{
  this->hasLayer_ = 0;
  this->numRectangles_ = 0;
}

void 
defiSlot_clearPoly(defiSlot * this)
{
  struct defiPoints *p;

  int     i;

  for (i = 0;
       i < this->numPolys_;
       i++) {
    p = this->polygons_[i];
    defFree((char *) (p->x));
    defFree((char *) (p->y));
    defFree((char *) (this->polygons_[i]));
  }
  this->numPolys_ = 0;
}

void 
defiSlot_Destroy(defiSlot * this)
{
  if (this->layerName_)
    defFree(this->layerName_);
  defFree((char *) (this->xl_));
  defFree((char *) (this->yl_));
  defFree((char *) (this->xh_));
  defFree((char *) (this->yh_));
  this->rectsAllocated_ = 0;
  this->xl_ = 0;
  this->yl_ = 0;
  this->xh_ = 0;
  this->yh_ = 0;
  defiSlot_clearPoly(this);
  if (this->polygons_)
    defFree((char *) (this->polygons_));
  this->polygons_ = 0;
  defiSlot_clear(this);
}

void 
defiSlot_setLayer(defiSlot * this,
		  const char *name)
{
  int     len = strlen(name) + 1;

  if (this->layerNameLength_ < len) {
    if (this->layerName_)
      defFree(this->layerName_);
    this->layerName_ = (char *) defMalloc(len);
    this->layerNameLength_ = len;
  }
  strcpy(this->layerName_, DEFCASE(name));
  this->hasLayer_ = 1;
}

void 
defiSlot_addRect(defiSlot * this,
		 int xl,
		 int yl,
		 int xh,
		 int yh)
{
  if (this->numRectangles_ == this->rectsAllocated_) {
    int     i;

    int     max = this->rectsAllocated_ = this->rectsAllocated_ * 2;

    int    *newxl = (int *) defMalloc(sizeof(int) * max);

    int    *newyl = (int *) defMalloc(sizeof(int) * max);

    int    *newxh = (int *) defMalloc(sizeof(int) * max);

    int    *newyh = (int *) defMalloc(sizeof(int) * max);

    for (i = 0;
	 i < this->numRectangles_;
	 i++) {
      newxl[i] = this->xl_[i];
      newyl[i] = this->yl_[i];
      newxh[i] = this->xh_[i];
      newyh[i] = this->yh_[i];
    }
    defFree((char *) (this->xl_));
    defFree((char *) (this->yl_));
    defFree((char *) (this->xh_));
    defFree((char *) (this->yh_));
    this->xl_ = newxl;
    this->yl_ = newyl;
    this->xh_ = newxh;
    this->yh_ = newyh;
  }
  this->xl_[this->numRectangles_] = xl;
  this->yl_[this->numRectangles_] = yl;
  this->xh_[this->numRectangles_] = xh;
  this->yh_[this->numRectangles_] = yh;
  this->numRectangles_ += 1;
}

/*
 * 5.6
 */ void 
defiSlot_addPolygon(defiSlot * this,
		    defiGeometries * geom)
{
  struct defiPoints *p;

  int     x, y;

  int     i;

  if (this->numPolys_ == this->polysAllocated_) {
    struct defiPoints **poly;

    this->polysAllocated_ = (this->polysAllocated_ == 0) ? 2 : this->polysAllocated_ * 2;
    poly = (struct defiPoints **) defMalloc(sizeof(struct defiPoints *) * this->polysAllocated_);
    for (i = 0;
	 i < this->numPolys_;
	 i++)
      poly[i] = this->polygons_[i];
    if (this->polygons_)
      defFree((char *) (this->polygons_));
    this->polygons_ = poly;
  }
  p = (struct defiPoints *) defMalloc(sizeof(struct defiPoints));
  p->numPoints = defiGeometries_numPoints(geom);
  p->x = (int *) defMalloc(sizeof(int) * p->numPoints);
  p->y = (int *) defMalloc(sizeof(int) * p->numPoints);
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
  this->polygons_[this->numPolys_] = p;
  this->numPolys_ += 1;
}

int 
defiSlot_hasLayer(const defiSlot * this)
{
  return this->hasLayer_;
}

const char *
defiSlot_layerName(const defiSlot * this)
{
  return this->layerName_;
}

int 
defiSlot_numRectangles(const defiSlot * this)
{
  return this->numRectangles_;
}

int 
defiSlot_xl(const defiSlot * this,
	    int index)
{
  char    msg[160];

  if (index < 0 || index >= this->numRectangles_) {
    sprintf(msg, "ERROR (DEFPARS-6160): The index number %d given for the SLOT RECTANGLE is invalid.\nValid index is from 0 to %d", index, this->numRectangles_);
    defiError(msg);
    return 0;
  }
  return this->xl_[index];
}

int 
defiSlot_yl(const defiSlot * this,
	    int index)
{
  char    msg[160];

  if (index < 0 || index >= this->numRectangles_) {
    sprintf(msg, "ERROR (DEFPARS-6160): The index number %d given for the SLOT RECTANGLE is invalid.\nValid index is from 0 to %d", index, this->numRectangles_);
    defiError(msg);
    return 0;
  }
  return this->yl_[index];
}

int 
defiSlot_xh(const defiSlot * this,
	    int index)
{
  char    msg[160];

  if (index < 0 || index >= this->numRectangles_) {
    sprintf(msg, "ERROR (DEFPARS-6160): The index number %d given for the SLOT RECTANGLE is invalid.\nValid index is from 0 to %d", index, this->numRectangles_);
    defiError(msg);
    return 0;
  }
  return this->xh_[index];
}

int 
defiSlot_yh(const defiSlot * this,
	    int index)
{
  char    msg[160];

  if (index < 0 || index >= this->numRectangles_) {
    sprintf(msg, "ERROR (DEFPARS-6160): The index number %d given for the SLOT RECTANGLE is invalid.\nValid index is from 0 to %d", index, this->numRectangles_);
    defiError(msg);
    return 0;
  }
  return this->yh_[index];
}

/*
 * 5.6
 */ int 
defiSlot_numPolygons(const defiSlot * this)
{
  return this->numPolys_;
}

/*
 * 5.6
 */ struct defiPoints 
defiSlot_getPolygon(const defiSlot * this,
		    int index)
{
  return *(this->polygons_[index]);
}

void 
defiSlot_print(const defiSlot * this,
	       FILE * f)
{
  int     i, j;

  struct defiPoints points;

  if (defiSlot_hasLayer(this))
    fprintf(f, "- LAYER %s\n", defiSlot_layerName(this));
  for (i = 0;
       i < defiSlot_numRectangles(this);
       i++) {
    fprintf(f, " RECT %d %d %d %d\n", defiSlot_xl(this,
						  i), defiSlot_yl(this,
						       i), defiSlot_xh(this,
						       i), defiSlot_yh(this,
								       i));
  }
  for (i = 0;
       i < defiSlot_numPolygons(this);
       i++) {
    fprintf(f, " POLYGON ");
    points = defiSlot_getPolygon(this,
				 i);
    for (j = 0;
	 j < points.numPoints;
	 j++)
      fprintf(f, "%d %d ", points.x[j], points.y[j]);
    fprintf(f, "\n");
  }
  fprintf(f, "\n");
}
