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
#include "lex.h"
#include "defiGroup.h"
#include "defiDebug.h"

/*
 * defiGroup
 */ defiGroup *
defiGroup_CreateUninitialized()
{
  defiGroup *this = (defiGroup *) malloc(sizeof(defiGroup));

  defiGroup_Init(this);
  return (this);
}

void 
defiGroup_Init(defiGroup * this)
{
  this->name_ = 0;
  this->nameLength_ = 0;
  this->region_ = 0;
  this->regionLength_ = 0;
  this->numRects_ = 0;
  this->rectsAllocated_ = 2;
  this->xl_ = (int *) defMalloc(sizeof(int) * 2);
  this->yl_ = (int *) defMalloc(sizeof(int) * 2);
  this->xh_ = (int *) defMalloc(sizeof(int) * 2);
  this->yh_ = (int *) defMalloc(sizeof(int) * 2);
  this->numProps_ = 0;
  this->propsAllocated_ = 2;
  this->propNames_ = (char **) defMalloc(sizeof(char *) * 2);
  this->propValues_ = (char **) defMalloc(sizeof(char *) * 2);
  this->propDValues_ = (double *) defMalloc(sizeof(double) * 2);
  this->propTypes_ = (char *) defMalloc(sizeof(char) * 2);
}

void
defiGroup_Delete(defiGroup * this)
{
  if (this == NULL)
    return;
  defiGroup_Destroy(this);
  free(this);
}

void 
defiGroup_Destroy(defiGroup * this)
{
  if (this->name_)
    defFree(this->name_);
  if (this->region_)
    defFree(this->region_);
  this->name_ = 0;
  this->nameLength_ = 0;
  this->region_ = 0;
  this->regionLength_ = 0;
  defiGroup_clear(this);
  defFree((char *) (this->propNames_));
  defFree((char *) (this->propValues_));
  defFree((char *) (this->propDValues_));
  defFree((char *) (this->propTypes_));
  defFree((char *) (this->xl_));
  defFree((char *) (this->yl_));
  defFree((char *) (this->xh_));
  defFree((char *) (this->yh_));
}

void 
defiGroup_clear(defiGroup * this)
{
  int     i;

  this->hasRegionName_ = 0;
  this->hasPerim_ = 0;
  this->hasMaxX_ = 0;
  this->hasMaxY_ = 0;
  for (i = 0;
       i < this->numProps_;
       i++) {
    defFree(this->propNames_[i]);
    defFree(this->propValues_[i]);
    this->propDValues_[i] = 0;
  }
  this->numProps_ = 0;
  this->numRects_ = 0;
}

void 
defiGroup_setup(defiGroup * this,
		const char *name)
{
  int     len = strlen(name) + 1;

  if (len > this->nameLength_) {
    if (this->name_)
      defFree(this->name_);
    this->nameLength_ = len;
    this->name_ = (char *) defMalloc(len);
  }
  strcpy(this->name_, DEFCASE(name));
  defiGroup_clear(this);
}

void 
defiGroup_addRegionRect(defiGroup * this,
			int xl,
			int yl,
			int xh,
			int yh)
{
  int     i;

  if (this->numRects_ == this->rectsAllocated_) {
    int     max = this->numRects_ * 2;

    int    *nxl = (int *) defMalloc(sizeof(int) * max);

    int    *nyl = (int *) defMalloc(sizeof(int) * max);

    int    *nxh = (int *) defMalloc(sizeof(int) * max);

    int    *nyh = (int *) defMalloc(sizeof(int) * max);

    max = this->numRects_;
    for (i = 0;
	 i < max;
	 i++) {
      nxl[i] = this->xl_[i];
      nyl[i] = this->yl_[i];
      nxh[i] = this->xh_[i];
      nyh[i] = this->yh_[i];
    }
    defFree((char *) (this->xl_));
    defFree((char *) (this->yl_));
    defFree((char *) (this->xh_));
    defFree((char *) (this->yh_));
    this->xl_ = nxl;
    this->yl_ = nyl;
    this->xh_ = nxh;
    this->yh_ = nyh;
    this->rectsAllocated_ *= 2;
  }
  i = this->numRects_;
  this->xl_[i] = xl;
  this->yl_[i] = yl;
  this->xh_[i] = xh;
  this->yh_[i] = yh;
  this->numRects_ += 1;
}

void 
defiGroup_regionRects(defiGroup * this,
		      int *size,
		      int **xl,
		      int **yl,
		      int **xh,
		      int **yh)
{
  *size = this->numRects_;
  *xl = this->xl_;
  *yl = this->yl_;
  *xh = this->xh_;
  *yh = this->yh_;
}

void 
defiGroup_setRegionName(defiGroup * this,
			const char *region)
{
  int     len = strlen(region) + 1;

  if (len > this->regionLength_) {
    if (this->region_)
      defFree(this->region_);
    this->regionLength_ = len;
    this->region_ = (char *) defMalloc(len);
  }
  strcpy(this->region_, DEFCASE(region));
  this->hasRegionName_ = 1;
}

void 
defiGroup_setMaxX(defiGroup * this,
		  int x)
{
  this->hasMaxX_ = 1;
  this->maxX_ = x;
}

void 
defiGroup_setMaxY(defiGroup * this,
		  int y)
{
  this->hasMaxY_ = 1;
  this->maxY_ = y;
}

void 
defiGroup_setPerim(defiGroup * this,
		   int p)
{
  this->hasPerim_ = 1;
  this->perim_ = p;
}

void 
defiGroup_addProperty(defiGroup * this,
		      const char *name,
		      const char *value,
		      const char type)
{
  int     len;

  if (this->numProps_ == this->propsAllocated_) {
    int     i;

    char  **nn;

    char  **nv;

    double *nd;

    char   *nt;

    this->propsAllocated_ *= 2;
    nn = (char **) defMalloc(sizeof(char *) * this->propsAllocated_);
    nv = (char **) defMalloc(sizeof(char *) * this->propsAllocated_);
    nd = (double *) defMalloc(sizeof(double) * this->propsAllocated_);
    nt = (char *) defMalloc(sizeof(char) * this->propsAllocated_);
    for (i = 0;
	 i < this->numProps_;
	 i++) {
      nn[i] = this->propNames_[i];
      nv[i] = this->propValues_[i];
      nd[i] = this->propDValues_[i];
      nt[i] = this->propTypes_[i];
    }
    defFree((char *) (this->propNames_));
    defFree((char *) (this->propValues_));
    defFree((char *) (this->propDValues_));
    defFree((char *) (this->propTypes_));
    this->propNames_ = nn;
    this->propValues_ = nv;
    this->propDValues_ = nd;
    this->propTypes_ = nt;
  }
  len = strlen(name) + 1;
  this->propNames_[this->numProps_] = (char *) defMalloc(len);
  strcpy(this->propNames_[this->numProps_], DEFCASE(name));
  len = strlen(value) + 1;
  this->propValues_[this->numProps_] = (char *) defMalloc(len);
  strcpy(this->propValues_[this->numProps_], DEFCASE(value));
  this->propDValues_[this->numProps_] = 0;
  this->propTypes_[this->numProps_] = type;
  this->numProps_ += 1;
}

void 
defiGroup_addNumProperty(defiGroup * this,
			 const char *name,
			 const double d,
			 const char *value,
			 const char type)
{
  int     len;

  if (this->numProps_ == this->propsAllocated_) {
    int     i;

    char  **nn;

    char  **nv;

    double *nd;

    char   *nt;

    this->propsAllocated_ *= 2;
    nn = (char **) defMalloc(sizeof(char *) * this->propsAllocated_);
    nv = (char **) defMalloc(sizeof(char *) * this->propsAllocated_);
    nd = (double *) defMalloc(sizeof(double) * this->propsAllocated_);
    nt = (char *) defMalloc(sizeof(char) * this->propsAllocated_);
    for (i = 0;
	 i < this->numProps_;
	 i++) {
      nn[i] = this->propNames_[i];
      nv[i] = this->propValues_[i];
      nd[i] = this->propDValues_[i];
      nt[i] = this->propTypes_[i];
    }
    defFree((char *) (this->propNames_));
    defFree((char *) (this->propValues_));
    defFree((char *) (this->propDValues_));
    defFree((char *) (this->propTypes_));
    this->propNames_ = nn;
    this->propValues_ = nv;
    this->propDValues_ = nd;
    this->propTypes_ = nt;
  }
  len = strlen(name) + 1;
  this->propNames_[this->numProps_] = (char *) defMalloc(len);
  strcpy(this->propNames_[this->numProps_], DEFCASE(name));
  len = strlen(value) + 1;
  this->propValues_[this->numProps_] = (char *) defMalloc(len);
  strcpy(this->propValues_[this->numProps_], DEFCASE(value));
  this->propDValues_[this->numProps_] = d;
  this->propTypes_[this->numProps_] = type;
  this->numProps_ += 1;
}

int 
defiGroup_numProps(const defiGroup * this)
{
  return this->numProps_;
}

const char *
defiGroup_propName(const defiGroup * this,
		   int index)
{
  char    msg[160];

  if (index < 0 || index >= this->numProps_) {
    sprintf(msg, "ERROR (LEFPARS-6050): The index number %d given for the GROUP PROPERTY is invalid.\nValid index is from 0 to %d", index, this->numProps_);
    defiError(msg);
    return 0;
  }
  return this->propNames_[index];
}

const char *
defiGroup_propValue(const defiGroup * this,
		    int index)
{
  char    msg[160];

  if (index < 0 || index >= this->numProps_) {
    sprintf(msg, "ERROR (LEFPARS-6050): The index number %d given for the GROUP PROPERTY is invalid.\nValid index is from 0 to %d", index, this->numProps_);
    defiError(msg);
    return 0;
  }
  return this->propValues_[index];
}

double 
defiGroup_propNumber(const defiGroup * this,
		     int index)
{
  char    msg[160];

  if (index < 0 || index >= this->numProps_) {
    sprintf(msg, "ERROR (LEFPARS-6050): The index number %d given for the GROUP PROPERTY is invalid.\nValid index is from 0 to %d", index, this->numProps_);
    defiError(msg);
    return 0;
  }
  return this->propDValues_[index];
}

const char 
defiGroup_propType(const defiGroup * this,
		   int index)
{
  char    msg[160];

  if (index < 0 || index >= this->numProps_) {
    sprintf(msg, "ERROR (LEFPARS-6050): The index number %d given for the GROUP PROPERTY is invalid.\nValid index is from 0 to %d", index, this->numProps_);
    defiError(msg);
    return 0;
  }
  return this->propTypes_[index];
}

int 
defiGroup_propIsNumber(const defiGroup * this,
		       int index)
{
  char    msg[160];

  if (index < 0 || index >= this->numProps_) {
    sprintf(msg, "ERROR (LEFPARS-6050): The index number %d given for the GROUP PROPERTY is invalid.\nValid index is from 0 to %d", index, this->numProps_);
    defiError(msg);
    return 0;
  }
  return this->propDValues_[index] ? 1 : 0;
}

int 
defiGroup_propIsString(const defiGroup * this,
		       int index)
{
  char    msg[160];

  if (index < 0 || index >= this->numProps_) {
    sprintf(msg, "ERROR (LEFPARS-6050): The index number %d given for the GROUP PROPERTY is invalid.\nValid index is from 0 to %d", index, this->numProps_);
    defiError(msg);
    return 0;
  }
  return this->propDValues_[index] ? 0 : 1;
}

const char *
defiGroup_regionName(const defiGroup * this)
{
  return this->region_;
}

const char *
defiGroup_name(const defiGroup * this)
{
  return this->name_;
}

int 
defiGroup_perim(const defiGroup * this)
{
  return this->perim_;
}

int 
defiGroup_maxX(const defiGroup * this)
{
  return this->maxX_;
}

int 
defiGroup_maxY(const defiGroup * this)
{
  return this->maxY_;
}

int 
defiGroup_hasMaxX(const defiGroup * this)
{
  return this->hasMaxX_;
}

int 
defiGroup_hasMaxY(const defiGroup * this)
{
  return this->hasMaxY_;
}

int 
defiGroup_hasPerim(const defiGroup * this)
{
  return this->hasPerim_;
}

int 
defiGroup_hasRegionBox(const defiGroup * this)
{
  return this->numRects_ ? 1 : 0;
}

int 
defiGroup_hasRegionName(const defiGroup * this)
{
  return this->hasRegionName_;
}

void 
defiGroup_print(const defiGroup * this,
		FILE * f)
{
  int     i;

  fprintf(f, "Group '%s'\n", defiGroup_name(this));
  if (defiGroup_hasRegionName(this)) {
    fprintf(f, " region name '%s'\n", defiGroup_regionName(this));
  }
  if (defiGroup_hasRegionBox(this)) {
    int     size = this->numRects_;

    int    *xl = this->xl_;

    int    *yl = this->yl_;

    int    *xh = this->xh_;

    int    *yh = this->yh_;

    for (i = 0;
	 i < size;
	 i++)
      fprintf(f, " region box %d,%d %d,%d\n", xl[i], yl[i], xh[i], yh[i]);
  }
  if (defiGroup_hasMaxX(this)) {
    fprintf(f, " max x %d\n", defiGroup_maxX(this));
  }
  if (defiGroup_hasMaxY(this)) {
    fprintf(f, " max y %d\n", defiGroup_maxY(this));
  }
  if (defiGroup_hasPerim(this)) {
    fprintf(f, " perim %d\n", defiGroup_perim(this));
  }
}
