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
#include "defiBlockage.h"
#include "defiDebug.h"

/*
 * defiBlockages
 */ defiBlockage *
defiBlockage_CreateUninitialized()
{
  defiBlockage *this = (defiBlockage *) malloc(sizeof(defiBlockage));

  defiBlockage_Init(this);
  return (this);
}

void 
defiBlockage_Init(defiBlockage * this)
{
  this->numPolys_ = 0;
  defiBlockage_clear(this);
  this->layerName_ = 0;
  this->componentName_ = 0;
  this->layerNameLength_ = 0;
  this->componentNameLength_ = 0;
  this->xl_ = 0;
  this->yl_ = 0;
  this->xh_ = 0;
  this->yh_ = 0;
  this->rectsAllocated_ = 0;
  this->polysAllocated_ = 0;
  this->polygons_ = 0;
}

void
defiBlockage_Delete(defiBlockage * this)
{
  if (this == NULL)
    return;
  defiBlockage_Destroy(this);
  free(this);
}

void 
defiBlockage_clear(defiBlockage * this)
{
  this->hasLayer_ = 0;
  this->hasPlacement_ = 0;
  this->hasComponent_ = 0;
  this->hasSlots_ = 0;
  this->hasFills_ = 0;
  this->hasPushdown_ = 0;
  this->hasExceptpgnet_ = 0;
  this->hasSoft_ = 0;
  this->maxDensity_ = -1;
  this->minSpacing_ = -1;
  this->width_ = -1;
  this->numRectangles_ = 0;
}

void 
defiBlockage_clearPoly(defiBlockage * this)
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
defiBlockage_Destroy(defiBlockage * this)
{
  if (this->layerName_)
    defFree(this->layerName_);
  if (this->componentName_)
    defFree(this->componentName_);
  this->layerName_ = 0;
  this->componentName_ = 0;
  if (this->rectsAllocated_) {
    defFree((char *) (this->xl_));
    defFree((char *) (this->yl_));
    defFree((char *) (this->xh_));
    defFree((char *) (this->yh_));
    this->rectsAllocated_ = 0;
    this->xl_ = 0;
    this->yl_ = 0;
    this->xh_ = 0;
    this->yh_ = 0;
  }
  defiBlockage_clearPoly(this);
  defFree((char *) (this->polygons_));
  this->polygons_ = 0;
  defiBlockage_clear(this);
}

void 
defiBlockage_setLayer(defiBlockage * this,
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
defiBlockage_setPlacement(defiBlockage * this)
{
  this->hasPlacement_ = 1;
  return;
}

void 
defiBlockage_setComponent(defiBlockage * this,
			  const char *name)
{
  int     len;

  len = strlen(name) + 1;
  if (this->componentNameLength_ < len) {
    if (this->componentName_)
      defFree(this->componentName_);
    this->componentName_ = (char *) defMalloc(len);
    this->componentNameLength_ = len;
  }
  strcpy(this->componentName_, DEFCASE(name));
  this->hasComponent_ = 1;
}

void 
defiBlockage_setSlots(defiBlockage * this)
{
  this->hasSlots_ = 1;
}

void 
defiBlockage_setFills(defiBlockage * this)
{
  this->hasFills_ = 1;
}

void 
defiBlockage_setPushdown(defiBlockage * this)
{
  this->hasPushdown_ = 1;
}

/*
 * 5.7
 */ void 
defiBlockage_setExceptpgnet(defiBlockage * this)
{
  this->hasExceptpgnet_ = 1;
}

/*
 * 5.7
 */ void 
defiBlockage_setSoft(defiBlockage * this)
{
  this->hasSoft_ = 1;
}

/*
 * 5.7
 */ void 
defiBlockage_setPartial(defiBlockage * this,
			double maxDensity)
{
  this->maxDensity_ = maxDensity;
}

void 
defiBlockage_setSpacing(defiBlockage * this,
			int minSpacing)
{
  this->minSpacing_ = minSpacing;
}

void 
defiBlockage_setDesignRuleWidth(defiBlockage * this,
				int width)
{
  this->width_ = width;
}

void 
defiBlockage_addRect(defiBlockage * this,
		     int xl,
		     int yl,
		     int xh,
		     int yh)
{
  if (this->numRectangles_ == this->rectsAllocated_) {
    int     i;

    int     max = this->rectsAllocated_ = (this->rectsAllocated_ == 0) ? 2 : this->rectsAllocated_ * 2;

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
defiBlockage_addPolygon(defiBlockage * this,
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
defiBlockage_hasLayer(const defiBlockage * this)
{
  return this->hasLayer_;
}

int 
defiBlockage_hasPlacement(const defiBlockage * this)
{
  return this->hasPlacement_;
}

int 
defiBlockage_hasComponent(const defiBlockage * this)
{
  return this->hasComponent_;
}

int 
defiBlockage_hasSlots(const defiBlockage * this)
{
  return this->hasSlots_;
}

int 
defiBlockage_hasFills(const defiBlockage * this)
{
  return this->hasFills_;
}

int 
defiBlockage_hasPushdown(const defiBlockage * this)
{
  return this->hasPushdown_;
}

/*
 * 5.7
 */ int 
defiBlockage_hasExceptpgnet(const defiBlockage * this)
{
  return this->hasExceptpgnet_;
}

/*
 * 5.7
 */ int 
defiBlockage_hasSoft(const defiBlockage * this)
{
  return this->hasSoft_;
}

/*
 * 5.7
 */ int 
defiBlockage_hasPartial(const defiBlockage * this)
{
  if (this->maxDensity_ == -1)
    return 0;
  return 1;
}

/*
 * 5.7
 */ double 
defiBlockage_placementMaxDensity(const defiBlockage * this)
{
  return this->maxDensity_;
}

int 
defiBlockage_hasSpacing(const defiBlockage * this)
{
  if (this->minSpacing_ == -1)
    return 0;
  return 1;
}

int 
defiBlockage_hasDesignRuleWidth(const defiBlockage * this)
{
  if (this->width_ == -1)
    return 0;
  return 1;
}

int 
defiBlockage_minSpacing(const defiBlockage * this)
{
  return this->minSpacing_;
}

int 
defiBlockage_designRuleWidth(const defiBlockage * this)
{
  return this->width_;
}

const char *
defiBlockage_layerName(const defiBlockage * this)
{
  return this->layerName_;
}

const char *
defiBlockage_layerComponentName(const defiBlockage * this)
{
  return this->componentName_;
}

const char *
defiBlockage_placementComponentName(const defiBlockage * this)
{
  return this->componentName_;
}

int 
defiBlockage_numRectangles(const defiBlockage * this)
{
  return this->numRectangles_;
}

int 
defiBlockage_xl(const defiBlockage * this,
		int index)
{
  if (index < 0 || index >= this->numRectangles_) {
    defiError("bad index for blockage xl");
    return 0;
  }
  return this->xl_[index];
}

int 
defiBlockage_yl(const defiBlockage * this,
		int index)
{
  if (index < 0 || index >= this->numRectangles_) {
    defiError("bad index for blockage yl");
    return 0;
  }
  return this->yl_[index];
}

int 
defiBlockage_xh(const defiBlockage * this,
		int index)
{
  if (index < 0 || index >= this->numRectangles_) {
    defiError("bad index for blockage xh");
    return 0;
  }
  return this->xh_[index];
}

int 
defiBlockage_yh(const defiBlockage * this,
		int index)
{
  if (index < 0 || index >= this->numRectangles_) {
    defiError("bad index for blockage yh");
    return 0;
  }
  return this->yh_[index];
}

/*
 * 5.6
 */ int 
defiBlockage_numPolygons(const defiBlockage * this)
{
  return this->numPolys_;
}

/*
 * 5.6
 */ struct defiPoints 
defiBlockage_getPolygon(const defiBlockage * this,
			int index)
{
  return *(this->polygons_[index]);
}

void 
defiBlockage_print(const defiBlockage * this,
		   FILE * f)
{
  int     i, j;

  struct defiPoints points;

  if (defiBlockage_hasLayer(this)) {
    fprintf(f, "- LAYER %s", defiBlockage_layerName(this));
    if (defiBlockage_hasComponent(this))
      fprintf(f, " + COMPONENT %s", defiBlockage_layerComponentName(this));
    if (defiBlockage_hasSlots(this))
      fprintf(f, " + SLOTS");
    if (defiBlockage_hasFills(this))
      fprintf(f, " + FILLS");
    if (defiBlockage_hasPushdown(this))
      fprintf(f, " + PUSHDOWN");
    if (defiBlockage_hasExceptpgnet(this))
      fprintf(f, " + EXCEPTPGNET");
    fprintf(f, "\n");
  }
  if (defiBlockage_hasPlacement(this)) {
    fprintf(f, "- PLACEMENT");
    if (defiBlockage_hasComponent(this))
      fprintf(f, " + COMPONENT %s", defiBlockage_layerComponentName(this));
    if (defiBlockage_hasPushdown(this))
      fprintf(f, " + PUSHDOWN");
    if (defiBlockage_hasSoft(this))
      fprintf(f, " + SOFT");
    if (defiBlockage_hasPartial(this))
      fprintf(f, " + PARTIAL", defiBlockage_placementMaxDensity(this));
    fprintf(f, "\n");
  }
  for (i = 0;
       i < defiBlockage_numRectangles(this);
       i++) {
    fprintf(f, " RECT %d %d %d %d\n", defiBlockage_xl(this,
						   i), defiBlockage_yl(this,
						   i), defiBlockage_xh(this,
						   i), defiBlockage_yh(this,
								       i));
  }
  for (i = 0;
       i < defiBlockage_numPolygons(this);
       i++) {
    fprintf(f, " POLYGON ");
    points = defiBlockage_getPolygon(this,
				     i);
    for (j = 0;
	 j < points.numPoints;
	 j++)
      fprintf(f, "%d %d ", points.x[j], points.y[j]);
    fprintf(f, "\n");
  }
  fprintf(f, "\n");
}
