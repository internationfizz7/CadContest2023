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
#include "defiFill.h"
#include "defiDebug.h"

/*
 * defiFill
 */ defiFill *
defiFill_CreateUninitialized()
{
  defiFill *this = (defiFill *) malloc(sizeof(defiFill));

  defiFill_Init(this);
  return (this);
}

void 
defiFill_Init(defiFill * this)
{
  this->numPolys_ = 0;
  this->numPts_ = 0;
  defiFill_clear(this);
  this->layerNameLength_ = 0;
  this->xl_ = (int *) malloc(sizeof(int) * 1);
  this->yl_ = (int *) malloc(sizeof(int) * 1);
  this->xh_ = (int *) malloc(sizeof(int) * 1);
  this->yh_ = (int *) malloc(sizeof(int) * 1);
  this->rectsAllocated_ = 1;
  this->polysAllocated_ = 0;
  this->polygons_ = 0;
  this->layerName_ = 0;
  this->viaName_ = 0;
  this->viaNameLength_ = 0;
  this->viaPts_ = 0;
  this->ptsAllocated_ = 0;
  this->viaPts_ = 0;
}

void
defiFill_Delete(defiFill * this)
{
  if (this == NULL)
    return;
  defiFill_Destroy(this);
  free(this);
}

void 
defiFill_clear(defiFill * this)
{
  this->hasLayer_ = 0;
  this->layerOpc_ = 0;
  this->numRectangles_ = 0;
  this->hasVia_ = 0;
  this->viaOpc_ = 0;
}

void 
defiFill_clearPoly(defiFill * this)
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
defiFill_clearPts(defiFill * this)
{
  struct defiPoints *p;

  int     i;

  for (i = 0;
       i < this->numPts_;
       i++) {
    p = this->viaPts_[i];
    defFree((char *) (p->x));
    defFree((char *) (p->y));
    defFree((char *) (this->viaPts_[i]));
  }
  this->numPts_ = 0;
}

void 
defiFill_Destroy(defiFill * this)
{
  if (this->layerName_)
    defFree(this->layerName_);
  if (this->viaName_)
    defFree(this->viaName_);
  defFree((char *) (this->xl_));
  defFree((char *) (this->yl_));
  defFree((char *) (this->xh_));
  defFree((char *) (this->yh_));
  this->rectsAllocated_ = 0;
  this->xl_ = 0;
  this->yl_ = 0;
  this->xh_ = 0;
  this->yh_ = 0;
  defiFill_clearPoly(this);
  if (this->polygons_)
    defFree((char *) (this->polygons_));
  this->polygons_ = 0;
  defiFill_clearPts(this);
  if (this->viaPts_)
    defFree((char *) (this->viaPts_));
  this->viaPts_ = 0;
  defiFill_clear(this);
}

void 
defiFill_setLayer(defiFill * this,
		  const char *name)
{
  int     len = strlen(name) + 1;

  if (this->layerNameLength_ < len) {
    if (this->layerName_)
      defFree(this->layerName_);
    this->layerName_ = (char *) malloc(len);
    this->layerNameLength_ = len;
  }
  strcpy(this->layerName_, DEFCASE(name));
  this->hasLayer_ = 1;
}

/*
 * 5.7
 */ void 
defiFill_setLayerOpc(defiFill * this)
{
  this->layerOpc_ = 1;
}

void 
defiFill_addRect(defiFill * this,
		 int xl,
		 int yl,
		 int xh,
		 int yh)
{
  if (this->numRectangles_ == this->rectsAllocated_) {
    int     i;

    int     max = this->rectsAllocated_ = this->rectsAllocated_ * 2;

    int    *newxl = (int *) malloc(sizeof(int) * max);

    int    *newyl = (int *) malloc(sizeof(int) * max);

    int    *newxh = (int *) malloc(sizeof(int) * max);

    int    *newyh = (int *) malloc(sizeof(int) * max);

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
defiFill_addPolygon(defiFill * this,
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
defiFill_hasLayer(const defiFill * this)
{
  return this->hasLayer_;
}

const char *
defiFill_layerName(const defiFill * this)
{
  return this->layerName_;
}

/*
 * 5.7
 */ int 
defiFill_hasLayerOpc(const defiFill * this)
{
  return this->layerOpc_;
}

int 
defiFill_numRectangles(const defiFill * this)
{
  return this->numRectangles_;
}

int 
defiFill_xl(const defiFill * this,
	    int index)
{
  if (index < 0 || index >= this->numRectangles_) {
    defiError("bad index for Fill xl");
    return 0;
  }
  return this->xl_[index];
}

int 
defiFill_yl(const defiFill * this,
	    int index)
{
  if (index < 0 || index >= this->numRectangles_) {
    defiError("bad index for Fill yl");
    return 0;
  }
  return this->yl_[index];
}

int 
defiFill_xh(const defiFill * this,
	    int index)
{
  if (index < 0 || index >= this->numRectangles_) {
    defiError("bad index for Fill xh");
    return 0;
  }
  return this->xh_[index];
}

int 
defiFill_yh(const defiFill * this,
	    int index)
{
  if (index < 0 || index >= this->numRectangles_) {
    defiError("bad index for Fill yh");
    return 0;
  }
  return this->yh_[index];
}

/*
 * 5.6
 */ int 
defiFill_numPolygons(const defiFill * this)
{
  return this->numPolys_;
}

/*
 * 5.6
 */ struct defiPoints 
defiFill_getPolygon(const defiFill * this,
		    int index)
{
  return *(this->polygons_[index]);
}

/*
 * 5.7
 */ void 
defiFill_setVia(defiFill * this,
		const char *name)
{
  int     len = strlen(name) + 1;

  if (this->viaNameLength_ < len) {
    if (this->viaName_)
      defFree(this->viaName_);
    this->viaName_ = (char *) malloc(len);
    this->viaNameLength_ = len;
  }
  strcpy(this->viaName_, DEFCASE(name));
  this->hasVia_ = 1;
}

/*
 * 5.7
 */ void 
defiFill_setViaOpc(defiFill * this)
{
  this->viaOpc_ = 1;
}

/*
 * 5.7
 */ void 
defiFill_addPts(defiFill * this,
		defiGeometries * geom)
{
  struct defiPoints *p;

  int     x, y;

  int     i;

  if (this->numPts_ == this->ptsAllocated_) {
    struct defiPoints **pts;

    this->ptsAllocated_ = (this->ptsAllocated_ == 0) ? 2 : this->ptsAllocated_ * 2;
    pts = (struct defiPoints **) defMalloc(sizeof(struct defiPoints *) * this->ptsAllocated_);
    for (i = 0;
	 i < this->numPts_;
	 i++)
      pts[i] = this->viaPts_[i];
    if (this->viaPts_)
      defFree((char *) (this->viaPts_));
    this->viaPts_ = pts;
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
  this->viaPts_[this->numPts_] = p;
  this->numPts_ += 1;
}

/*
 * 5.7
 */ int 
defiFill_hasVia(const defiFill * this)
{
  return this->hasVia_;
}

/*
 * 5.7
 */ const char *
defiFill_viaName(const defiFill * this)
{
  return this->viaName_;
}

/*
 * 5.7
 */ int 
defiFill_hasViaOpc(const defiFill * this)
{
  return this->viaOpc_;
}

/*
 * 5.7
 */ int 
defiFill_numViaPts(const defiFill * this)
{
  return this->numPts_;
}

/*
 * 5.7
 */ struct defiPoints 
defiFill_getViaPts(const defiFill * this,
		   int index)
{
  return *(this->viaPts_[index]);
}

void 
defiFill_print(const defiFill * this,
	       FILE * f)
{
  int     i, j;

  struct defiPoints points;

  if (defiFill_hasLayer(this))
    fprintf(f, "- LAYER %s", defiFill_layerName(this));
  if (defiFill_hasLayerOpc(this))
    fprintf(f, " + OPC");
  fprintf(f, "\n");
  for (i = 0;
       i < defiFill_numRectangles(this);
       i++) {
    fprintf(f, " RECT %d %d %d %d\n", defiFill_xl(this,
						  i), defiFill_yl(this,
						       i), defiFill_xh(this,
						       i), defiFill_yh(this,
								       i));
  }
  for (i = 0;
       i < defiFill_numPolygons(this);
       i++) {
    fprintf(f, " POLYGON ");
    points = defiFill_getPolygon(this,
				 i);
    for (j = 0;
	 j < points.numPoints;
	 j++)
      fprintf(f, "%d %d ", points.x[j], points.y[j]);
    fprintf(f, "\n");
  }
  fprintf(f, "\n");
  if (defiFill_hasVia(this))
    fprintf(f, "- VIA %s", defiFill_viaName(this));
  if (defiFill_hasViaOpc(this))
    fprintf(f, " + OPC");
  fprintf(f, "\n");
  for (i = 0;
       i < defiFill_numViaPts(this);
       i++) {
    fprintf(f, " ");
    points = defiFill_getViaPts(this,
				i);
    for (j = 0;
	 j < points.numPoints;
	 j++)
      fprintf(f, "%d %d ", points.x[j], points.y[j]);
    fprintf(f, "\n");
  }
  fprintf(f, "\n");
}
