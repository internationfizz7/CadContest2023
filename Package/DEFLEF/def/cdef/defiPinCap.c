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
#include "defiPinCap.h"
#include "defiComponent.h"
#include "defiDebug.h"
#include "defiUtil.h"

/*
 * defiPinCap
 */ void 
defiPinCap_setPin(defiPinCap * this,
		  int p)
{
  this->pin_ = p;
}

void 
defiPinCap_setCap(defiPinCap * this,
		  double d)
{
  this->cap_ = d;
}

int 
defiPinCap_pin(const defiPinCap * this)
{
  return this->pin_;
}

double 
defiPinCap_cap(const defiPinCap * this)
{
  return this->cap_;
}

void 
defiPinCap_print(const defiPinCap * this,
		 FILE * f)
{
  fprintf(f, "PinCap %d %5.2f\n", this->pin_, this->cap_);
}

/*
 * defiPinAntennaModel
 */ defiPinAntennaModel *
defiPinAntennaModel_CreateUninitialized()
{
  defiPinAntennaModel *this = (defiPinAntennaModel *) malloc(sizeof(defiPinAntennaModel));

  defiPinAntennaModel_Init(this);
  return (this);
}

void 
defiPinAntennaModel_Init(defiPinAntennaModel * this)
{
  this->numAPinGateArea_ = 0;
  this->APinGateAreaAllocated_ = 0;
  this->APinGateArea_ = 0;
  this->APinGateAreaLayer_ = 0;
  this->numAPinMaxAreaCar_ = 0;
  this->APinMaxAreaCarAllocated_ = 0;
  this->APinMaxAreaCar_ = 0;
  this->APinMaxAreaCarLayer_ = 0;
  this->numAPinMaxSideAreaCar_ = 0;
  this->APinMaxSideAreaCarAllocated_ = 0;
  this->APinMaxSideAreaCar_ = 0;
  this->APinMaxSideAreaCarLayer_ = 0;
  this->numAPinMaxCutCar_ = 0;
  this->APinMaxCutCarAllocated_ = 0;
  this->APinMaxCutCar_ = 0;
  this->APinMaxCutCarLayer_ = 0;
}

void
defiPinAntennaModel_Delete(defiPinAntennaModel * this)
{
  if (this == NULL)
    return;
  defiPinAntennaModel_Destroy(this);
  free(this);
}

void 
defiPinAntennaModel_clear(defiPinAntennaModel * this)
{
  int     i;

  if (this->oxide_)
    defFree((char *) (this->oxide_));
  this->oxide_ = 0;
  for (i = 0;
       i < this->numAPinGateArea_;
       i++) {
    if (this->APinGateAreaLayer_[i])
      defFree(this->APinGateAreaLayer_[i]);
  }
  this->numAPinGateArea_ = 0;
  for (i = 0;
       i < this->numAPinMaxAreaCar_;
       i++) {
    if (this->APinMaxAreaCarLayer_[i])
      defFree(this->APinMaxAreaCarLayer_[i]);
  }
  this->numAPinMaxAreaCar_ = 0;
  for (i = 0;
       i < this->numAPinMaxSideAreaCar_;
       i++) {
    if (this->APinMaxSideAreaCarLayer_[i])
      defFree(this->APinMaxSideAreaCarLayer_[i]);
  }
  this->numAPinMaxSideAreaCar_ = 0;
  for (i = 0;
       i < this->numAPinMaxCutCar_;
       i++) {
    if (this->APinMaxCutCarLayer_[i])
      defFree(this->APinMaxCutCarLayer_[i]);
  }
  this->numAPinMaxCutCar_ = 0;
}

void 
defiPinAntennaModel_Destroy(defiPinAntennaModel * this)
{
  defiPinAntennaModel_clear(this);
  if (this->APinGateArea_)
    defFree((char *) (this->APinGateArea_));
  if (this->APinGateAreaLayer_)
    defFree((char *) (this->APinGateAreaLayer_));
  if (this->APinMaxAreaCar_)
    defFree((char *) (this->APinMaxAreaCar_));
  if (this->APinMaxAreaCarLayer_)
    defFree((char *) (this->APinMaxAreaCarLayer_));
  if (this->APinMaxSideAreaCar_)
    defFree((char *) (this->APinMaxSideAreaCar_));
  if (this->APinMaxSideAreaCarLayer_)
    defFree((char *) (this->APinMaxSideAreaCarLayer_));
  if (this->APinMaxCutCar_)
    defFree((char *) (this->APinMaxCutCar_));
  if (this->APinMaxCutCarLayer_)
    defFree((char *) (this->APinMaxCutCarLayer_));
}

/*
 * 5.5
 */ void 
defiPinAntennaModel_setAntennaModel(defiPinAntennaModel * this,
				    int aOxide)
{
  switch (aOxide) {
    case 2:this->oxide_ = strdup("OXIDE2");
    break;
  case 3:
    this->oxide_ = strdup("OXIDE3");
    break;
  case 4:
    this->oxide_ = strdup("OXIDE4");
    break;
  default:
    this->oxide_ = strdup("OXIDE1");
    break;
  }
}

void 
defiPinAntennaModel_addAPinGateArea(defiPinAntennaModel * this,
				    int value,
				    const char *layer)
{
  if (this->numAPinGateArea_ == this->APinGateAreaAllocated_) {
    int     i;

    int     max;

    int     lim = this->numAPinGateArea_;

    int    *nd;

    char  **nl;

    if (this->APinGateAreaAllocated_ == 0)
      max = this->APinGateAreaAllocated_ = 2;
    else
      max = this->APinGateAreaAllocated_ *= 2;
    nd = (int *) defMalloc(sizeof(int) * max);
    nl = (char **) defMalloc(sizeof(char *) * max);
    for (i = 0;
	 i < lim;
	 i++) {
      nd[i] = this->APinGateArea_[i];
      nl[i] = this->APinGateAreaLayer_[i];
    }
    defFree((char *) (this->APinGateArea_));
    defFree((char *) (this->APinGateAreaLayer_));
    this->APinGateArea_ = nd;
    this->APinGateAreaLayer_ = nl;
  }
  this->APinGateArea_[this->numAPinGateArea_] = value;
  if (layer) {
    this->APinGateAreaLayer_[this->numAPinGateArea_] = (char *) defMalloc(strlen(layer) + 1);
    strcpy(this->APinGateAreaLayer_[this->numAPinGateArea_], DEFCASE(layer));
  }
  else
    this->APinGateAreaLayer_[this->numAPinGateArea_] = NULL;
  this->numAPinGateArea_ += 1;
}

void 
defiPinAntennaModel_addAPinMaxAreaCar(defiPinAntennaModel * this,
				      int value,
				      const char *layer)
{
  if (this->numAPinMaxAreaCar_ == this->APinMaxAreaCarAllocated_) {
    int     i;

    int     max;

    int     lim = this->numAPinMaxAreaCar_;

    int    *nd;

    char  **nl;

    if (this->APinMaxAreaCarAllocated_ == 0)
      max = this->APinMaxAreaCarAllocated_ = 2;
    else
      max = this->APinMaxAreaCarAllocated_ *= 2;
    nd = (int *) defMalloc(sizeof(int) * max);
    nl = (char **) defMalloc(sizeof(char *) * max);
    for (i = 0;
	 i < lim;
	 i++) {
      nd[i] = this->APinMaxAreaCar_[i];
      nl[i] = this->APinMaxAreaCarLayer_[i];
    }
    defFree((char *) (this->APinMaxAreaCar_));
    defFree((char *) (this->APinMaxAreaCarLayer_));
    this->APinMaxAreaCar_ = nd;
    this->APinMaxAreaCarLayer_ = nl;
  }
  this->APinMaxAreaCar_[this->numAPinMaxAreaCar_] = value;
  if (layer) {
    this->APinMaxAreaCarLayer_[this->numAPinMaxAreaCar_] = (char *) defMalloc(strlen(layer) + 1);
    strcpy(this->APinMaxAreaCarLayer_[this->numAPinMaxAreaCar_], DEFCASE(layer));
  }
  else
    this->APinMaxAreaCarLayer_[this->numAPinMaxAreaCar_] = NULL;
  this->numAPinMaxAreaCar_ += 1;
}

void 
defiPinAntennaModel_addAPinMaxSideAreaCar(defiPinAntennaModel * this,
					  int value,
					  const char *layer)
{
  if (this->numAPinMaxSideAreaCar_ == this->APinMaxSideAreaCarAllocated_) {
    int     i;

    int     max;

    int     lim = this->numAPinMaxSideAreaCar_;

    int    *nd;

    char  **nl;

    if (this->APinMaxSideAreaCarAllocated_ == 0)
      max = this->APinMaxSideAreaCarAllocated_ = 2;
    else
      max = this->APinMaxSideAreaCarAllocated_ *= 2;
    nd = (int *) defMalloc(sizeof(int) * max);
    nl = (char **) defMalloc(sizeof(char *) * max);
    for (i = 0;
	 i < lim;
	 i++) {
      nd[i] = this->APinMaxSideAreaCar_[i];
      nl[i] = this->APinMaxSideAreaCarLayer_[i];
    }
    defFree((char *) (this->APinMaxSideAreaCar_));
    defFree((char *) (this->APinMaxSideAreaCarLayer_));
    this->APinMaxSideAreaCar_ = nd;
    this->APinMaxSideAreaCarLayer_ = nl;
  }
  this->APinMaxSideAreaCar_[this->numAPinMaxSideAreaCar_] = value;
  if (layer) {
    this->APinMaxSideAreaCarLayer_[this->numAPinMaxSideAreaCar_] = (char *) defMalloc(strlen(layer) + 1);
    strcpy(this->APinMaxSideAreaCarLayer_[this->numAPinMaxSideAreaCar_], DEFCASE(layer));
  }
  else
    this->APinMaxSideAreaCarLayer_[this->numAPinMaxSideAreaCar_] = NULL;
  this->numAPinMaxSideAreaCar_ += 1;
}

void 
defiPinAntennaModel_addAPinMaxCutCar(defiPinAntennaModel * this,
				     int value,
				     const char *layer)
{
  if (this->numAPinMaxCutCar_ == this->APinMaxCutCarAllocated_) {
    int     i;

    int     max;

    int     lim = this->numAPinMaxCutCar_;

    int    *nd;

    char  **nl;

    if (this->APinMaxCutCarAllocated_ == 0)
      max = this->APinMaxCutCarAllocated_ = 2;
    else
      max = this->APinMaxCutCarAllocated_ *= 2;
    nd = (int *) defMalloc(sizeof(int) * max);
    nl = (char **) defMalloc(sizeof(char *) * max);
    for (i = 0;
	 i < lim;
	 i++) {
      nd[i] = this->APinMaxCutCar_[i];
      nl[i] = this->APinMaxCutCarLayer_[i];
    }
    defFree((char *) (this->APinMaxCutCar_));
    defFree((char *) (this->APinMaxCutCarLayer_));
    this->APinMaxCutCar_ = nd;
    this->APinMaxCutCarLayer_ = nl;
  }
  this->APinMaxCutCar_[this->numAPinMaxCutCar_] = value;
  if (layer) {
    this->APinMaxCutCarLayer_[this->numAPinMaxCutCar_] = (char *) defMalloc(strlen(layer) + 1);
    strcpy(this->APinMaxCutCarLayer_[this->numAPinMaxCutCar_], DEFCASE(layer));
  }
  else
    this->APinMaxCutCarLayer_[this->numAPinMaxCutCar_] = NULL;
  this->numAPinMaxCutCar_ += 1;
}

/*
 * 5.5
 */ char *
defiPinAntennaModel_antennaOxide(const defiPinAntennaModel * this)
{
  return this->oxide_;
}

int 
defiPinAntennaModel_hasAPinGateArea(const defiPinAntennaModel * this)
{
  return this->numAPinGateArea_ ? 1 : 0;
}

int 
defiPinAntennaModel_hasAPinMaxAreaCar(const defiPinAntennaModel * this)
{
  return this->numAPinMaxAreaCar_ ? 1 : 0;
}

int 
defiPinAntennaModel_hasAPinMaxSideAreaCar(const defiPinAntennaModel * this)
{
  return this->numAPinMaxSideAreaCar_ ? 1 : 0;
}

int 
defiPinAntennaModel_hasAPinMaxCutCar(const defiPinAntennaModel * this)
{
  return this->numAPinMaxCutCar_ ? 1 : 0;
}

int 
defiPinAntennaModel_numAPinGateArea(const defiPinAntennaModel * this)
{
  return this->numAPinGateArea_;
}

int 
defiPinAntennaModel_numAPinMaxAreaCar(const defiPinAntennaModel * this)
{
  return this->numAPinMaxAreaCar_;
}

int 
defiPinAntennaModel_numAPinMaxSideAreaCar(const defiPinAntennaModel * this)
{
  return this->numAPinMaxSideAreaCar_;
}

int 
defiPinAntennaModel_numAPinMaxCutCar(const defiPinAntennaModel * this)
{
  return this->numAPinMaxCutCar_;
}

int 
defiPinAntennaModel_APinGateArea(const defiPinAntennaModel * this,
				 int i)
{
  return this->APinGateArea_[i];
}

int 
defiPinAntennaModel_hasAPinGateAreaLayer(const defiPinAntennaModel * this,
					 int i)
{
  return (this->APinGateAreaLayer_[i] && *(this->APinGateAreaLayer_[i])) ? 1 : 0;
}

const char *
defiPinAntennaModel_APinGateAreaLayer(const defiPinAntennaModel * this,
				      int i)
{
  return this->APinGateAreaLayer_[i];
}

int 
defiPinAntennaModel_APinMaxAreaCar(const defiPinAntennaModel * this,
				   int i)
{
  return this->APinMaxAreaCar_[i];
}

int 
defiPinAntennaModel_hasAPinMaxAreaCarLayer(const defiPinAntennaModel * this,
					   int i)
{
  return (this->APinMaxAreaCarLayer_[i] && *(this->APinMaxAreaCarLayer_[i])) ? 1 : 0;
}

const char *
defiPinAntennaModel_APinMaxAreaCarLayer(const defiPinAntennaModel * this,
					int i)
{
  return this->APinMaxAreaCarLayer_[i];
}

int 
defiPinAntennaModel_APinMaxSideAreaCar(const defiPinAntennaModel * this,
				       int i)
{
  return this->APinMaxSideAreaCar_[i];
}

int 
defiPinAntennaModel_hasAPinMaxSideAreaCarLayer(const defiPinAntennaModel * this,
					       int i)
{
  return (this->APinMaxSideAreaCarLayer_[i] && *(this->APinMaxSideAreaCarLayer_[i])) ? 1 : 0;
}

const char *
defiPinAntennaModel_APinMaxSideAreaCarLayer(const defiPinAntennaModel * this,
					    int i)
{
  return this->APinMaxSideAreaCarLayer_[i];
}

int 
defiPinAntennaModel_APinMaxCutCar(const defiPinAntennaModel * this,
				  int i)
{
  return this->APinMaxCutCar_[i];
}

int 
defiPinAntennaModel_hasAPinMaxCutCarLayer(const defiPinAntennaModel * this,
					  int i)
{
  return (this->APinMaxCutCarLayer_[i] && *(this->APinMaxCutCarLayer_[i])) ? 1 : 0;
}

const char *
defiPinAntennaModel_APinMaxCutCarLayer(const defiPinAntennaModel * this,
				       int i)
{
  return this->APinMaxCutCarLayer_[i];
}

/*
 * defiPinPort
 */ defiPinPort *
defiPinPort_CreateUninitialized()
{
  defiPinPort *this = (defiPinPort *) malloc(sizeof(defiPinPort));

  defiPinPort_Init(this);
  return (this);
}

void 
defiPinPort_Init(defiPinPort * this)
{
  this->layersAllocated_ = 0;
  this->numLayers_ = 0;
  this->layers_ = 0;
  this->layerMinSpacing_ = 0;
  this->layerEffectiveWidth_ = 0;
  this->xl_ = 0;
  this->yl_ = 0;
  this->xh_ = 0;
  this->yh_ = 0;
  this->polysAllocated_ = 0;
  this->numPolys_ = 0;
  this->polygonNames_ = 0;
  this->polyMinSpacing_ = 0;
  this->polyEffectiveWidth_ = 0;
  this->polygons_ = 0;
  this->viasAllocated_ = 0;
  this->numVias_ = 0;
  this->viaNames_ = 0;
  this->viaX_ = 0;
  this->viaY_ = 0;
  this->placeType_ = 0;
  this->x_ = 0;
  this->y_ = 0;
  this->orient_ = 0;
}

void
defiPinPort_Delete(defiPinPort * this)
{
  if (this == NULL)
    return;
  defiPinPort_clear(this);
  free(this);
}

void 
defiPinPort_clear(defiPinPort * this)
{
  int     i;

  this->placeType_ = 0;
  this->orient_ = 0;
  this->x_ = 0;
  this->y_ = 0;
  if (this->layers_) {
    for (i = 0;
	 i < this->numLayers_;
	 i++)
      if (this->layers_[i])
	defFree(this->layers_[i]);
    defFree((char *) (this->layers_));
    defFree((char *) (this->xl_));
    defFree((char *) (this->yl_));
    defFree((char *) (this->xh_));
    defFree((char *) (this->yh_));
    defFree((char *) (this->layerMinSpacing_));
    defFree((char *) (this->layerEffectiveWidth_));
  }
  this->layers_ = 0;
  this->layerMinSpacing_ = 0;
  this->layerEffectiveWidth_ = 0;
  this->numLayers_ = 0;
  this->layersAllocated_ = 0;
  if (this->polygonNames_) {
    struct defiPoints *p;

    for (i = 0;
	 i < this->numPolys_;
	 i++) {
      if (this->polygonNames_[i])
	defFree((char *) (this->polygonNames_[i]));
      p = this->polygons_[i];
      defFree((char *) (p->x));
      defFree((char *) (p->y));
      defFree((char *) (this->polygons_[i]));
    }
    defFree((char *) (this->polygonNames_));
    defFree((char *) (this->polygons_));
    defFree((char *) (this->polyMinSpacing_));
    defFree((char *) (this->polyEffectiveWidth_));
    this->polygonNames_ = 0;
    this->polygons_ = 0;
    this->polyMinSpacing_ = 0;
    this->polyEffectiveWidth_ = 0;
  }
  this->numPolys_ = 0;
  this->polysAllocated_ = 0;
  if (this->viaNames_) {
    for (i = 0;
	 i < this->numVias_;
	 i++)
      if (this->viaNames_[i])
	defFree(this->viaNames_[i]);
    defFree((char *) (this->viaNames_));
    defFree((char *) (this->viaX_));
    defFree((char *) (this->viaY_));
  }
  this->viaNames_ = 0;
  this->numVias_ = 0;
  this->viasAllocated_ = 0;
}

void 
defiPinPort_addLayer(defiPinPort * this,
		     const char *layer)
{
  if (this->numLayers_ >= this->layersAllocated_) {
    int     i;

    char  **newl;

    int    *nxl, *nyl, *nxh, *nyh;

    int    *lms, *lew;

    this->layersAllocated_ = this->layersAllocated_ ? this->layersAllocated_ * 2 : 8;
    newl = (char **) defMalloc(this->layersAllocated_ * sizeof(char *));
    nxl = (int *) defMalloc(this->layersAllocated_ * sizeof(int));
    nyl = (int *) defMalloc(this->layersAllocated_ * sizeof(int));
    nxh = (int *) defMalloc(this->layersAllocated_ * sizeof(int));
    nyh = (int *) defMalloc(this->layersAllocated_ * sizeof(int));
    lms = (int *) defMalloc(this->layersAllocated_ * sizeof(int));
    lew = (int *) defMalloc(this->layersAllocated_ * sizeof(int));
    for (i = 0;
	 i < this->numLayers_;
	 i++) {
      newl[i] = this->layers_[i];
      nxl[i] = this->xl_[i];
      nyl[i] = this->yl_[i];
      nxh[i] = this->xh_[i];
      nyh[i] = this->yh_[i];
      lms[i] = this->layerMinSpacing_[i];
      lew[i] = this->layerEffectiveWidth_[i];
    }
    if (this->numLayers_ > 0) {
      defFree((char *) this->layers_);
      defFree((char *) this->xl_);
      defFree((char *) this->yl_);
      defFree((char *) this->xh_);
      defFree((char *) this->yh_);
      defFree((char *) this->layerMinSpacing_);
      defFree((char *) this->layerEffectiveWidth_);
    }
    this->layers_ = newl;
    this->xl_ = nxl;
    this->yl_ = nyl;
    this->xh_ = nxh;
    this->yh_ = nyh;
    this->layerMinSpacing_ = lms;
    this->layerEffectiveWidth_ = lew;
  }
  this->layers_[this->numLayers_] = (char *) defMalloc(strlen(layer) + 1);
  strcpy(this->layers_[this->numLayers_], DEFCASE(layer));
  this->xl_[this->numLayers_] = 0;
  this->yl_[this->numLayers_] = 0;
  this->xh_[this->numLayers_] = 0;
  this->yh_[this->numLayers_] = 0;
  this->layerMinSpacing_[this->numLayers_] = -1;
  this->layerEffectiveWidth_[this->numLayers_] = -1;
  this->numLayers_ += 1;
}

void 
defiPinPort_addLayerSpacing(defiPinPort * this,
			    int minSpacing)
{
  this->layerMinSpacing_[this->numLayers_ - 1] = minSpacing;
}

void 
defiPinPort_addLayerDesignRuleWidth(defiPinPort * this,
				    int effectiveWidth)
{
  this->layerEffectiveWidth_[this->numLayers_ - 1] = effectiveWidth;
}

void 
defiPinPort_addLayerPts(defiPinPort * this,
			int xl,
			int yl,
			int xh,
			int yh)
{
  this->xl_[this->numLayers_ - 1] = xl;
  this->yl_[this->numLayers_ - 1] = yl;
  this->xh_[this->numLayers_ - 1] = xh;
  this->yh_[this->numLayers_ - 1] = yh;
}

void 
defiPinPort_addPolygon(defiPinPort * this,
		       const char *layerName)
{
  int    *pms, *pdw;

  int     i;

  if (this->numPolys_ == this->polysAllocated_) {
    char  **newn;

    struct defiPoints **poly;

    this->polysAllocated_ = (this->polysAllocated_ == 0) ? 2 : this->polysAllocated_ * 2;
    newn = (char **) defMalloc(sizeof(char *) * this->polysAllocated_);
    poly = (struct defiPoints **) defMalloc(sizeof(struct defiPoints *) * this->polysAllocated_);
    pms = (int *) defMalloc(this->polysAllocated_ * sizeof(int));
    pdw = (int *) defMalloc(this->polysAllocated_ * sizeof(int));
    for (i = 0;
	 i < this->numPolys_;
	 i++) {
      newn[i] = this->polygonNames_[i];
      poly[i] = this->polygons_[i];
      pms[i] = this->polyMinSpacing_[i];
      pdw[i] = this->polyEffectiveWidth_[i];
    }
    if (this->numPolys_ > 0) {
      defFree((char *) (this->polygons_));
      defFree((char *) (this->polygonNames_));
      defFree((char *) (this->polyMinSpacing_));
      defFree((char *) (this->polyEffectiveWidth_));
    }
    this->polygonNames_ = newn;
    this->polygons_ = poly;
    this->polyMinSpacing_ = pms;
    this->polyEffectiveWidth_ = pdw;
  }
  this->polygonNames_[this->numPolys_] = strdup(layerName);
  this->polygons_[this->numPolys_] = 0;
  this->polyMinSpacing_[this->numPolys_] = -1;
  this->polyEffectiveWidth_[this->numPolys_] = -1;
  this->numPolys_ += 1;
}

void 
defiPinPort_addPolySpacing(defiPinPort * this,
			   int minSpacing)
{
  this->polyMinSpacing_[this->numPolys_ - 1] = minSpacing;
}

void 
defiPinPort_addPolyDesignRuleWidth(defiPinPort * this,
				   int effectiveWidth)
{
  this->polyEffectiveWidth_[this->numPolys_ - 1] = effectiveWidth;
}

void 
defiPinPort_addPolygonPts(defiPinPort * this,
			  defiGeometries * geom)
{
  struct defiPoints *p;

  int     x, y;

  int     i;

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
  this->polygons_[this->numPolys_ - 1] = p;
}

void 
defiPinPort_addVia(defiPinPort * this,
		   const char *viaName,
		   int ptX,
		   int ptY)
{
  if (this->numVias_ >= this->viasAllocated_) {
    int     i;

    char  **newl;

    int    *nx, *ny;

    this->viasAllocated_ = this->viasAllocated_ ? this->viasAllocated_ * 2 : 8;
    newl = (char **) defMalloc(this->viasAllocated_ * sizeof(char *));
    nx = (int *) defMalloc(this->viasAllocated_ * sizeof(int));
    ny = (int *) defMalloc(this->viasAllocated_ * sizeof(int));
    for (i = 0;
	 i < this->numVias_;
	 i++) {
      newl[i] = this->viaNames_[i];
      nx[i] = this->viaX_[i];
      ny[i] = this->viaY_[i];
    }
    if (this->numVias_ > 0) {
      defFree((char *) this->viaNames_);
      defFree((char *) this->viaX_);
      defFree((char *) this->viaY_);
    }
    this->viaNames_ = newl;
    this->viaX_ = nx;
    this->viaY_ = ny;
  }
  this->viaNames_[this->numVias_] = (char *) defMalloc(strlen(viaName) + 1);
  strcpy(this->viaNames_[this->numVias_], DEFCASE(viaName));
  this->viaX_[this->numVias_] = ptX;
  this->viaY_[this->numVias_] = ptY;
  this->numVias_ += 1;
}

void 
defiPinPort_setPlacement(defiPinPort * this,
			 int typ,
			 int x,
			 int y,
			 int orient)
{
  this->x_ = x;
  this->y_ = y;
  this->orient_ = orient;
  this->placeType_ = typ;
}

int 
defiPinPort_numLayer(const defiPinPort * this)
{
  return this->numLayers_;
}

const char *
defiPinPort_layer(const defiPinPort * this,
		  int index)
{
  return this->layers_[index];
}

void 
defiPinPort_bounds(const defiPinPort * this,
		   int index,
		   int *xl,
		   int *yl,
		   int *xh,
		   int *yh)
{
  if (xl)
    *xl = this->xl_[index];
  if (yl)
    *yl = this->yl_[index];
  if (xh)
    *xh = this->xh_[index];
  if (yh)
    *yh = this->yh_[index];
}

int 
defiPinPort_hasLayerSpacing(const defiPinPort * this,
			    int index)
{
  if (this->layerMinSpacing_[index] == -1)
    return 0;
  return 1;
}

int 
defiPinPort_hasLayerDesignRuleWidth(const defiPinPort * this,
				    int index)
{
  if (this->layerEffectiveWidth_[index] == -1)
    return 0;
  return 1;
}

int 
defiPinPort_layerSpacing(const defiPinPort * this,
			 int index)
{
  return this->layerMinSpacing_[index];
}

int 
defiPinPort_layerDesignRuleWidth(const defiPinPort * this,
				 int index)
{
  return this->layerEffectiveWidth_[index];
}

int 
defiPinPort_numPolygons(const defiPinPort * this)
{
  return this->numPolys_;
}

const char *
defiPinPort_polygonName(const defiPinPort * this,
			int index)
{
  if (index < 0 || index > this->numPolys_) {
    defiError("index out of bounds");
    return 0;
  }
  return this->polygonNames_[index];
}

struct defiPoints 
defiPinPort_getPolygon(const defiPinPort * this,
		       int index)
{
  return *(this->polygons_[index]);
}

int 
defiPinPort_hasPolygonSpacing(const defiPinPort * this,
			      int index)
{
  if (this->polyMinSpacing_[index] == -1)
    return 0;
  return 1;
}

int 
defiPinPort_hasPolygonDesignRuleWidth(const defiPinPort * this,
				      int index)
{
  if (this->polyEffectiveWidth_[index] == -1)
    return 0;
  return 1;
}

int 
defiPinPort_polygonSpacing(const defiPinPort * this,
			   int index)
{
  return this->polyMinSpacing_[index];
}

int 
defiPinPort_polygonDesignRuleWidth(const defiPinPort * this,
				   int index)
{
  return this->polyEffectiveWidth_[index];
}

int 
defiPinPort_numVias(const defiPinPort * this)
{
  return this->numVias_;
}

const char *
defiPinPort_viaName(const defiPinPort * this,
		    int index)
{
  if (index < 0 || index > this->numVias_) {
    defiError("index out of bounds");
    return 0;
  }
  return this->viaNames_[index];
}

int 
defiPinPort_viaPtX(const defiPinPort * this,
		   int index)
{
  return this->viaX_[index];
}

int 
defiPinPort_viaPtY(const defiPinPort * this,
		   int index)
{
  return this->viaY_[index];
}

int 
defiPinPort_hasPlacement(const defiPinPort * this)
{
  return this->placeType_ == 0 ? 0 : 1;
}

int 
defiPinPort_isPlaced(const defiPinPort * this)
{
  return this->placeType_ == DEFI_COMPONENT_PLACED ? 1 : 0;
}

int 
defiPinPort_isCover(const defiPinPort * this)
{
  return this->placeType_ == DEFI_COMPONENT_COVER ? 1 : 0;
}

int 
defiPinPort_isFixed(const defiPinPort * this)
{
  return this->placeType_ == DEFI_COMPONENT_FIXED ? 1 : 0;
}

int 
defiPinPort_placementX(const defiPinPort * this)
{
  return this->x_;
}

int 
defiPinPort_placementY(const defiPinPort * this)
{
  return this->y_;
}

int 
defiPinPort_orient(const defiPinPort * this)
{
  return this->orient_;
}

const char *
defiPinPort_orientStr(const defiPinPort * this)
{
  return (defiOrientStr(this->orient_));
}

/*
 * defiPin
 */ defiPin *
defiPin_CreateUninitialized()
{
  defiPin *this = (defiPin *) malloc(sizeof(defiPin));

  defiPin_Init(this);
  return (this);
}

void 
defiPin_Init(defiPin * this)
{
  this->pinNameLength_ = 0;
  this->pinName_ = 0;
  this->netNameLength_ = 0;
  this->netName_ = 0;
  this->useLength_ = 0;
  this->use_ = 0;
  this->directionLength_ = 0;
  this->direction_ = 0;
  this->hasDirection_ = 0;
  this->hasUse_ = 0;
  this->placeType_ = 0;
  this->orient_ = 0;
  this->xl_ = 0;
  this->yl_ = 0;
  this->xh_ = 0;
  this->yh_ = 0;
  this->x_ = 0;
  this->y_ = 0;
  this->netExprLength_ = 0;
  this->netExpr_ = 0;
  this->hasNetExpr_ = 0;
  this->supplySensLength_ = 0;
  this->supplySens_ = 0;
  this->hasSupplySens_ = 0;
  this->groundSensLength_ = 0;
  this->groundSens_ = 0;
  this->hasGroundSens_ = 0;
  this->layers_ = 0;
  this->layersAllocated_ = 0;
  this->numLayers_ = 0;
  this->polygonNames_ = 0;
  this->numPolys_ = 0;
  this->polysAllocated_ = 0;
  this->polygons_ = 0;
  this->numAPinPartialMetalArea_ = 0;
  this->APinPartialMetalAreaAllocated_ = 0;
  this->APinPartialMetalArea_ = 0;
  this->APinPartialMetalAreaLayer_ = 0;
  this->numAPinPartialMetalSideArea_ = 0;
  this->APinPartialMetalSideAreaAllocated_ = 0;
  this->APinPartialMetalSideArea_ = 0;
  this->APinPartialMetalSideAreaLayer_ = 0;
  this->numAPinDiffArea_ = 0;
  this->APinDiffAreaAllocated_ = 0;
  this->APinDiffArea_ = 0;
  this->APinDiffAreaLayer_ = 0;
  this->numAPinPartialCutArea_ = 0;
  this->APinPartialCutAreaAllocated_ = 0;
  this->APinPartialCutArea_ = 0;
  this->APinPartialCutAreaLayer_ = 0;
  this->antennaModel_ = 0;
  this->viaNames_ = 0;
  this->viasAllocated_ = 0;
  this->numVias_ = 0;
  this->viaX_ = 0;
  this->viaY_ = 0;
  this->numPorts_ = 0;
  this->pinPort_ = 0;
  this->numAntennaModel_ = 0;
  this->antennaModelAllocated_ = 0;
}

void
defiPin_Delete(defiPin * this)
{
  if (this == NULL)
    return;
  defiPin_Destroy(this);
  free(this);
}

void 
defiPin_clear(defiPin * this)
{
  int     i;

  defiPinAntennaModel *aModel;

  this->hasDirection_ = 0;
  this->hasNetExpr_ = 0;
  this->hasSupplySens_ = 0;
  this->hasGroundSens_ = 0;
  this->hasUse_ = 0;
  this->hasSpecial_ = 0;
  this->placeType_ = 0;
  this->orient_ = 0;
  this->x_ = 0;
  this->y_ = 0;
  if (this->layers_) {
    for (i = 0;
	 i < this->numLayers_;
	 i++)
      if (this->layers_[i])
	defFree(this->layers_[i]);
    defFree((char *) (this->layers_));
    defFree((char *) (this->xl_));
    defFree((char *) (this->yl_));
    defFree((char *) (this->xh_));
    defFree((char *) (this->yh_));
    defFree((char *) (this->layerMinSpacing_));
    defFree((char *) (this->layerEffectiveWidth_));
  }
  this->layers_ = 0;
  this->layerMinSpacing_ = 0;
  this->layerEffectiveWidth_ = 0;
  this->numLayers_ = 0;
  this->layersAllocated_ = 0;
  if (this->polygonNames_) {
    struct defiPoints *p;

    for (i = 0;
	 i < this->numPolys_;
	 i++) {
      if (this->polygonNames_[i])
	defFree((char *) (this->polygonNames_[i]));
      p = this->polygons_[i];
      defFree((char *) (p->x));
      defFree((char *) (p->y));
      defFree((char *) (this->polygons_[i]));
    }
    defFree((char *) (this->polygonNames_));
    defFree((char *) (this->polygons_));
    defFree((char *) (this->polyMinSpacing_));
    defFree((char *) (this->polyEffectiveWidth_));
    this->polygonNames_ = 0;
    this->polygons_ = 0;
    this->polyMinSpacing_ = 0;
    this->polyEffectiveWidth_ = 0;
  }
  this->numPolys_ = 0;
  this->polysAllocated_ = 0;
  if (this->viaNames_) {
    for (i = 0;
	 i < this->numVias_;
	 i++)
      if (this->viaNames_[i])
	defFree(this->viaNames_[i]);
    defFree((char *) (this->viaNames_));
    defFree((char *) (this->viaX_));
    defFree((char *) (this->viaY_));
  }
  this->viaNames_ = 0;
  this->numVias_ = 0;
  this->viasAllocated_ = 0;
  if (this->pinPort_) {
    for (i = 0;
	 i < this->numPorts_;
	 i++) {
      if (this->pinPort_[i]) {
	defiPinPort_clear(this->pinPort_[i]);
	defFree(this->pinPort_[i]);
      }
    }
    defFree(this->pinPort_);
  }
  this->pinPort_ = 0;
  this->numPorts_ = 0;
  this->portsAllocated_ = 0;
  for (i = 0;
       i < this->numAPinPartialMetalArea_;
       i++) {
    if (this->APinPartialMetalAreaLayer_[i])
      defFree(this->APinPartialMetalAreaLayer_[i]);
  }
  this->numAPinPartialMetalArea_ = 0;
  for (i = 0;
       i < this->numAPinPartialMetalSideArea_;
       i++) {
    if (this->APinPartialMetalSideAreaLayer_[i])
      defFree(this->APinPartialMetalSideAreaLayer_[i]);
  }
  this->numAPinPartialMetalSideArea_ = 0;
  for (i = 0;
       i < this->numAPinDiffArea_;
       i++) {
    if (this->APinDiffAreaLayer_[i])
      defFree(this->APinDiffAreaLayer_[i]);
  }
  this->numAPinDiffArea_ = 0;
  for (i = 0;
       i < this->numAPinPartialCutArea_;
       i++) {
    if (this->APinPartialCutAreaLayer_[i])
      defFree(this->APinPartialCutAreaLayer_[i]);
  }
  this->numAPinPartialCutArea_ = 0;
  for (i = 0;
       i < this->antennaModelAllocated_;
       i++) {
    aModel = this->antennaModel_[i];
    if (i < this->numAntennaModel_) {
      defiPinAntennaModel_Destroy(aModel);
    }
    defFree(aModel);
  }
  this->numAntennaModel_ = 0;
  this->antennaModelAllocated_ = 0;
}

void 
defiPin_Destroy(defiPin * this)
{
  if (this->pinName_)
    defFree(this->pinName_);
  if (this->netName_)
    defFree(this->netName_);
  if (this->use_)
    defFree(this->use_);
  if (this->direction_)
    defFree(this->direction_);
  if (this->netExpr_)
    defFree(this->netExpr_);
  if (this->supplySens_)
    defFree(this->supplySens_);
  if (this->groundSens_)
    defFree(this->groundSens_);
  this->pinName_ = 0;
  this->netName_ = 0;
  this->use_ = 0;
  this->direction_ = 0;
  this->netExpr_ = 0;
  this->supplySens_ = 0;
  this->groundSens_ = 0;
  this->pinNameLength_ = 0;
  this->netNameLength_ = 0;
  this->useLength_ = 0;
  this->directionLength_ = 0;
  this->netExprLength_ = 0;
  this->supplySensLength_ = 0;
  this->groundSensLength_ = 0;
  this->layersAllocated_ = 0;
  defiPin_clear(this);
  if (this->APinPartialMetalArea_)
    defFree((char *) (this->APinPartialMetalArea_));
  if (this->APinPartialMetalAreaLayer_)
    defFree((char *) (this->APinPartialMetalAreaLayer_));
  if (this->APinPartialMetalSideArea_)
    defFree((char *) (this->APinPartialMetalSideArea_));
  if (this->APinPartialMetalSideAreaLayer_)
    defFree((char *) (this->APinPartialMetalSideAreaLayer_));
  if (this->APinDiffArea_)
    defFree((char *) (this->APinDiffArea_));
  if (this->APinDiffAreaLayer_)
    defFree((char *) (this->APinDiffAreaLayer_));
  if (this->APinPartialCutArea_)
    defFree((char *) (this->APinPartialCutArea_));
  if (this->APinPartialCutAreaLayer_)
    defFree((char *) (this->APinPartialCutAreaLayer_));
  if (this->antennaModel_)
    defFree((char *) (this->antennaModel_));
}

void 
defiPin_Setup(defiPin * this,
	      const char *pinName,
	      const char *netName)
{
  int     len = strlen(pinName) + 1;

  if (this->pinNameLength_ < len) {
    if (this->pinName_)
      defFree(this->pinName_);
    this->pinName_ = (char *) defMalloc(len);
    this->pinNameLength_ = len;
  }
  strcpy(this->pinName_, DEFCASE(pinName));
  len = strlen(netName) + 1;
  if (this->netNameLength_ < len) {
    if (this->netName_)
      defFree(this->netName_);
    this->netName_ = (char *) defMalloc(len);
    this->netNameLength_ = len;
  }
  strcpy(this->netName_, DEFCASE(netName));
  defiPin_clear(this);
}

void 
defiPin_setDirection(defiPin * this,
		     const char *dir)
{
  int     len = strlen(dir) + 1;

  if (this->directionLength_ < len) {
    if (this->direction_)
      defFree(this->direction_);
    this->direction_ = (char *) defMalloc(len);
    this->directionLength_ = len;
  }
  strcpy(this->direction_, DEFCASE(dir));
  this->hasDirection_ = 1;
}

void 
defiPin_setNetExpr(defiPin * this,
		   const char *name)
{
  int     len = strlen(name) + 1;

  if (this->netExprLength_ < len) {
    if (this->netExpr_)
      defFree(this->netExpr_);
    this->netExpr_ = (char *) defMalloc(len);
    this->netExprLength_ = len;
  }
  strcpy(this->netExpr_, DEFCASE(name));
  this->hasNetExpr_ = 1;
}

void 
defiPin_setSupplySens(defiPin * this,
		      const char *name)
{
  int     len = strlen(name) + 1;

  if (this->supplySensLength_ < len) {
    if (this->supplySens_)
      defFree(this->supplySens_);
    this->supplySens_ = (char *) defMalloc(len);
    this->supplySensLength_ = len;
  }
  strcpy(this->supplySens_, DEFCASE(name));
  this->hasSupplySens_ = 1;
}

void 
defiPin_setGroundSens(defiPin * this,
		      const char *name)
{
  int     len = strlen(name) + 1;

  if (this->groundSensLength_ < len) {
    if (this->groundSens_)
      defFree(this->groundSens_);
    this->groundSens_ = (char *) defMalloc(len);
    this->groundSensLength_ = len;
  }
  strcpy(this->groundSens_, DEFCASE(name));
  this->hasGroundSens_ = 1;
}

void 
defiPin_setUse(defiPin * this,
	       const char *use)
{
  int     len = strlen(use) + 1;

  if (this->useLength_ < len) {
    if (this->use_)
      defFree(this->use_);
    this->use_ = (char *) defMalloc(len);
    this->useLength_ = len;
  }
  strcpy(this->use_, DEFCASE(use));
  this->hasUse_ = 1;
}

/*
 * 5.6, renamed from setLayer to addLayer for multiple layers allowed
 */ void 
defiPin_addLayer(defiPin * this,
		 const char *layer)
{
  if (this->numLayers_ >= this->layersAllocated_) {
    int     i;

    char  **newl;

    int    *nxl, *nyl, *nxh, *nyh;

    int    *lms, *lew;

    this->layersAllocated_ = this->layersAllocated_ ? this->layersAllocated_ * 2 : 8;
    newl = (char **) defMalloc(this->layersAllocated_ * sizeof(char *));
    nxl = (int *) defMalloc(this->layersAllocated_ * sizeof(int));
    nyl = (int *) defMalloc(this->layersAllocated_ * sizeof(int));
    nxh = (int *) defMalloc(this->layersAllocated_ * sizeof(int));
    nyh = (int *) defMalloc(this->layersAllocated_ * sizeof(int));
    lms = (int *) defMalloc(this->layersAllocated_ * sizeof(int));
    lew = (int *) defMalloc(this->layersAllocated_ * sizeof(int));
    for (i = 0;
	 i < this->numLayers_;
	 i++) {
      newl[i] = this->layers_[i];
      nxl[i] = this->xl_[i];
      nyl[i] = this->yl_[i];
      nxh[i] = this->xh_[i];
      nyh[i] = this->yh_[i];
      lms[i] = this->layerMinSpacing_[i];
      lew[i] = this->layerEffectiveWidth_[i];
    }
    if (this->numLayers_ > 0) {
      defFree((char *) this->layers_);
      defFree((char *) this->xl_);
      defFree((char *) this->yl_);
      defFree((char *) this->xh_);
      defFree((char *) this->yh_);
      defFree((char *) this->layerMinSpacing_);
      defFree((char *) this->layerEffectiveWidth_);
    }
    this->layers_ = newl;
    this->xl_ = nxl;
    this->yl_ = nyl;
    this->xh_ = nxh;
    this->yh_ = nyh;
    this->layerMinSpacing_ = lms;
    this->layerEffectiveWidth_ = lew;
  }
  this->layers_[this->numLayers_] = (char *) defMalloc(strlen(layer) + 1);
  strcpy(this->layers_[this->numLayers_], DEFCASE(layer));
  this->xl_[this->numLayers_] = 0;
  this->yl_[this->numLayers_] = 0;
  this->xh_[this->numLayers_] = 0;
  this->yh_[this->numLayers_] = 0;
  this->layerMinSpacing_[this->numLayers_] = -1;
  this->layerEffectiveWidth_[this->numLayers_] = -1;
  this->numLayers_ += 1;
}

/*
 * 5.6
 */ void 
defiPin_addLayerPts(defiPin * this,
		    int xl,
		    int yl,
		    int xh,
		    int yh)
{
  this->xl_[this->numLayers_ - 1] = xl;
  this->yl_[this->numLayers_ - 1] = yl;
  this->xh_[this->numLayers_ - 1] = xh;
  this->yh_[this->numLayers_ - 1] = yh;
}

/*
 * 5.6
 */ void 
defiPin_addLayerSpacing(defiPin * this,
			int minSpacing)
{
  this->layerMinSpacing_[this->numLayers_ - 1] = minSpacing;
}

/*
 * 5.6
 */ void 
defiPin_addLayerDesignRuleWidth(defiPin * this,
				int effectiveWidth)
{
  this->layerEffectiveWidth_[this->numLayers_ - 1] = effectiveWidth;
}

void 
defiPin_setPlacement(defiPin * this,
		     int typ,
		     int x,
		     int y,
		     int orient)
{
  this->x_ = x;
  this->y_ = y;
  this->orient_ = orient;
  this->placeType_ = typ;
}

const char *
defiPin_pinName(const defiPin * this)
{
  return this->pinName_;
}

const char *
defiPin_netName(const defiPin * this)
{
  return this->netName_;
}

void 
defiPin_changePinName(defiPin * this,
		      const char *pinName)
{
  int     len = strlen(pinName) + 1;

  if (this->pinNameLength_ < len) {
    if (this->pinName_)
      defFree(this->pinName_);
    this->pinName_ = (char *) defMalloc(len);
    this->pinNameLength_ = len;
  }
  strcpy(this->pinName_, DEFCASE(pinName));
}

int 
defiPin_hasDirection(const defiPin * this)
{
  return (int) (this->hasDirection_);
}

int 
defiPin_hasUse(const defiPin * this)
{
  return (int) (this->hasUse_);
}

int 
defiPin_hasLayer(const defiPin * this)
{
  if (this->numLayers_ || this->numPolys_)
    return 1;
  else
    return 0;
}

int 
defiPin_hasPlacement(const defiPin * this)
{
  return this->placeType_ == 0 ? 0 : 1;
}

int 
defiPin_isUnplaced(const defiPin * this)
{
  return this->placeType_ == DEFI_COMPONENT_UNPLACED ? 1 : 0;
}

int 
defiPin_isPlaced(const defiPin * this)
{
  return this->placeType_ == DEFI_COMPONENT_PLACED ? 1 : 0;
}

int 
defiPin_isCover(const defiPin * this)
{
  return this->placeType_ == DEFI_COMPONENT_COVER ? 1 : 0;
}

int 
defiPin_isFixed(const defiPin * this)
{
  return this->placeType_ == DEFI_COMPONENT_FIXED ? 1 : 0;
}

int 
defiPin_placementX(const defiPin * this)
{
  return this->x_;
}

int 
defiPin_placementY(const defiPin * this)
{
  return this->y_;
}

const char *
defiPin_direction(const defiPin * this)
{
  return this->direction_;
}

const char *
defiPin_use(const defiPin * this)
{
  return this->use_;
}

int 
defiPin_numLayer(const defiPin * this)
{
  return this->numLayers_;
}

const char *
defiPin_layer(const defiPin * this,
	      int index)
{
  return this->layers_[index];
}

void 
defiPin_bounds(const defiPin * this,
	       int index,
	       int *xl,
	       int *yl,
	       int *xh,
	       int *yh)
{
  if (xl)
    *xl = this->xl_[index];
  if (yl)
    *yl = this->yl_[index];
  if (xh)
    *xh = this->xh_[index];
  if (yh)
    *yh = this->yh_[index];
}

/*
 * 5.6
 */ int 
defiPin_hasLayerSpacing(const defiPin * this,
			int index)
{
  if (this->layerMinSpacing_[index] == -1)
    return 0;
  return 1;
}

/*
 * 5.6
 */ int 
defiPin_hasLayerDesignRuleWidth(const defiPin * this,
				int index)
{
  if (this->layerEffectiveWidth_[index] == -1)
    return 0;
  return 1;
}

/*
 * 5.6
 */ int 
defiPin_layerSpacing(const defiPin * this,
		     int index)
{
  return this->layerMinSpacing_[index];
}

/*
 * 5.6
 */ int 
defiPin_layerDesignRuleWidth(const defiPin * this,
			     int index)
{
  return this->layerEffectiveWidth_[index];
}

int 
defiPin_orient(const defiPin * this)
{
  return this->orient_;
}

const char *
defiPin_orientStr(const defiPin * this)
{
  return (defiOrientStr(this->orient_));
}

void 
defiPin_setSpecial(defiPin * this)
{
  this->hasSpecial_ = 1;
}

/*
 * 5.5
 */ void 
defiPin_addAntennaModel(defiPin * this,
			int oxide)
{
  defiPinAntennaModel *amo;

  int     i;

  if (this->numAntennaModel_ == 0) {
    if (!this->antennaModel_)
      this->antennaModel_ = (defiPinAntennaModel **) defMalloc(sizeof(defiPinAntennaModel *) * 4);
    this->antennaModelAllocated_ = 4;
    for (i = 0;
	 i < 4;
	 i++) {
      this->antennaModel_[i] = (defiPinAntennaModel *) defMalloc(sizeof(defiPinAntennaModel));
    }
    this->numAntennaModel_++;
    this->antennaModelAllocated_ = 4;
    amo = this->antennaModel_[0];
  }
  else {
    amo = this->antennaModel_[this->numAntennaModel_];
    this->numAntennaModel_++;
  }
  defiPinAntennaModel_Init(amo);
  defiPinAntennaModel_setAntennaModel(amo,
				      oxide);
  return;
}

/*
 * 5.5
 */ int 
defiPin_numAntennaModel(const defiPin * this)
{
  return this->numAntennaModel_;
}

/*
 * 5.5
 */ defiPinAntennaModel *
defiPin_antennaModel(const defiPin * this,
		     int index)
{
  return this->antennaModel_[index];
}

void 
defiPin_addAPinPartialMetalArea(defiPin * this,
				int value,
				const char *layer)
{
  if (this->numAPinPartialMetalArea_ == this->APinPartialMetalAreaAllocated_) {
    int     i;

    int     max;

    int     lim = this->numAPinPartialMetalArea_;

    int    *nd;

    char  **nl;

    if (this->APinPartialMetalAreaAllocated_ == 0)
      max = this->APinPartialMetalAreaAllocated_ = 2;
    else
      max = this->APinPartialMetalAreaAllocated_ *= 2;
    nd = (int *) defMalloc(sizeof(int) * max);
    nl = (char **) defMalloc(sizeof(char *) * max);
    for (i = 0;
	 i < lim;
	 i++) {
      nd[i] = this->APinPartialMetalArea_[i];
      nl[i] = this->APinPartialMetalAreaLayer_[i];
    }
    defFree((char *) (this->APinPartialMetalArea_));
    defFree((char *) (this->APinPartialMetalAreaLayer_));
    this->APinPartialMetalArea_ = nd;
    this->APinPartialMetalAreaLayer_ = nl;
  }
  this->APinPartialMetalArea_[this->numAPinPartialMetalArea_] = value;
  if (layer) {
    this->APinPartialMetalAreaLayer_[this->numAPinPartialMetalArea_] = (char *) defMalloc(strlen(layer) + 1);
    strcpy(this->APinPartialMetalAreaLayer_[this->numAPinPartialMetalArea_], DEFCASE(layer));
  }
  else
    this->APinPartialMetalAreaLayer_[this->numAPinPartialMetalArea_] = NULL;
  this->numAPinPartialMetalArea_ += 1;
}

void 
defiPin_addAPinPartialMetalSideArea(defiPin * this,
				    int value,
				    const char *layer)
{
  if (this->numAPinPartialMetalSideArea_ == this->APinPartialMetalSideAreaAllocated_) {
    int     i;

    int     max;

    int     lim = this->numAPinPartialMetalSideArea_;

    int    *nd;

    char  **nl;

    if (this->APinPartialMetalSideAreaAllocated_ == 0)
      max = this->APinPartialMetalSideAreaAllocated_ = 2;
    else
      max = this->APinPartialMetalSideAreaAllocated_ *= 2;
    nd = (int *) defMalloc(sizeof(int) * max);
    nl = (char **) defMalloc(sizeof(char *) * max);
    for (i = 0;
	 i < lim;
	 i++) {
      nd[i] = this->APinPartialMetalSideArea_[i];
      nl[i] = this->APinPartialMetalSideAreaLayer_[i];
    }
    defFree((char *) (this->APinPartialMetalSideArea_));
    defFree((char *) (this->APinPartialMetalSideAreaLayer_));
    this->APinPartialMetalSideArea_ = nd;
    this->APinPartialMetalSideAreaLayer_ = nl;
  }
  this->APinPartialMetalSideArea_[this->numAPinPartialMetalSideArea_] = value;
  if (layer) {
    this->APinPartialMetalSideAreaLayer_[this->numAPinPartialMetalSideArea_] = (char *) defMalloc(strlen(layer) + 1);
    strcpy(this->APinPartialMetalSideAreaLayer_[this->numAPinPartialMetalSideArea_], DEFCASE(layer));
  }
  else
    this->APinPartialMetalSideAreaLayer_[this->numAPinPartialMetalSideArea_] = NULL;
  this->numAPinPartialMetalSideArea_ += 1;
}

void 
defiPin_addAPinGateArea(defiPin * this,
			int value,
			const char *layer)
{
  if (this->numAntennaModel_ == 0)
    defiPin_addAntennaModel(this,
			    1);
  defiPinAntennaModel_addAPinGateArea(this->antennaModel_[this->numAntennaModel_ - 1],
				      value,
				      layer);
}

void 
defiPin_addAPinDiffArea(defiPin * this,
			int value,
			const char *layer)
{
  if (this->numAPinDiffArea_ == this->APinDiffAreaAllocated_) {
    int     i;

    int     max;

    int     lim = this->numAPinDiffArea_;

    int    *nd;

    char  **nl;

    if (this->APinDiffAreaAllocated_ == 0)
      max = this->APinDiffAreaAllocated_ = 2;
    else
      max = this->APinDiffAreaAllocated_ *= 2;
    nd = (int *) defMalloc(sizeof(int) * max);
    nl = (char **) defMalloc(sizeof(char *) * max);
    for (i = 0;
	 i < lim;
	 i++) {
      nd[i] = this->APinDiffArea_[i];
      nl[i] = this->APinDiffAreaLayer_[i];
    }
    defFree((char *) (this->APinDiffArea_));
    defFree((char *) (this->APinDiffAreaLayer_));
    this->APinDiffArea_ = nd;
    this->APinDiffAreaLayer_ = nl;
  }
  this->APinDiffArea_[this->numAPinDiffArea_] = value;
  if (layer) {
    this->APinDiffAreaLayer_[this->numAPinDiffArea_] = (char *) defMalloc(strlen(layer) + 1);
    strcpy(this->APinDiffAreaLayer_[this->numAPinDiffArea_], DEFCASE(layer));
  }
  else
    this->APinDiffAreaLayer_[this->numAPinDiffArea_] = NULL;
  this->numAPinDiffArea_ += 1;
}

void 
defiPin_addAPinMaxAreaCar(defiPin * this,
			  int value,
			  const char *layer)
{
  if (this->numAntennaModel_ == 0)
    defiPin_addAntennaModel(this,
			    1);
  defiPinAntennaModel_addAPinMaxAreaCar(this->antennaModel_[this->numAntennaModel_ - 1],
					value,
					layer);
}

void 
defiPin_addAPinMaxSideAreaCar(defiPin * this,
			      int value,
			      const char *layer)
{
  if (this->numAntennaModel_ == 0)
    defiPin_addAntennaModel(this,
			    1);
  defiPinAntennaModel_addAPinMaxSideAreaCar(this->antennaModel_[this->numAntennaModel_ - 1],
					    value,
					    layer);
}

void 
defiPin_addAPinPartialCutArea(defiPin * this,
			      int value,
			      const char *layer)
{
  if (this->numAPinPartialCutArea_ == this->APinPartialCutAreaAllocated_) {
    int     i;

    int     max;

    int     lim = this->numAPinPartialCutArea_;

    int    *nd;

    char  **nl;

    if (this->APinPartialCutAreaAllocated_ == 0)
      max = this->APinPartialCutAreaAllocated_ = 2;
    else
      max = this->APinPartialCutAreaAllocated_ *= 2;
    nd = (int *) defMalloc(sizeof(int) * max);
    nl = (char **) defMalloc(sizeof(char *) * max);
    for (i = 0;
	 i < lim;
	 i++) {
      nd[i] = this->APinPartialCutArea_[i];
      nl[i] = this->APinPartialCutAreaLayer_[i];
    }
    defFree((char *) (this->APinPartialCutArea_));
    defFree((char *) (this->APinPartialCutAreaLayer_));
    this->APinPartialCutArea_ = nd;
    this->APinPartialCutAreaLayer_ = nl;
  }
  this->APinPartialCutArea_[this->numAPinPartialCutArea_] = value;
  if (layer) {
    this->APinPartialCutAreaLayer_[this->numAPinPartialCutArea_] = (char *) defMalloc(strlen(layer) + 1);
    strcpy(this->APinPartialCutAreaLayer_[this->numAPinPartialCutArea_], DEFCASE(layer));
  }
  else
    this->APinPartialCutAreaLayer_[this->numAPinPartialCutArea_] = NULL;
  this->numAPinPartialCutArea_ += 1;
}

void 
defiPin_addAPinMaxCutCar(defiPin * this,
			 int value,
			 const char *layer)
{
  if (this->numAntennaModel_ == 0)
    defiPin_addAntennaModel(this,
			    1);
  defiPinAntennaModel_addAPinMaxCutCar(this->antennaModel_[this->numAntennaModel_ - 1],
				       value,
				       layer);
}

int 
defiPin_hasSpecial(const defiPin * this)
{
  return (int) this->hasSpecial_;
}

int 
defiPin_hasAPinPartialMetalArea(const defiPin * this)
{
  return this->numAPinPartialMetalArea_ ? 1 : 0;
}

int 
defiPin_hasAPinPartialMetalSideArea(const defiPin * this)
{
  return this->numAPinPartialMetalSideArea_ ? 1 : 0;
}

int 
defiPin_hasAPinDiffArea(const defiPin * this)
{
  return this->numAPinDiffArea_ ? 1 : 0;
}

int 
defiPin_hasAPinPartialCutArea(const defiPin * this)
{
  return this->numAPinPartialCutArea_ ? 1 : 0;
}

int 
defiPin_numAPinPartialMetalArea(const defiPin * this)
{
  return this->numAPinPartialMetalArea_;
}

int 
defiPin_numAPinPartialMetalSideArea(const defiPin * this)
{
  return this->numAPinPartialMetalSideArea_;
}

int 
defiPin_numAPinDiffArea(const defiPin * this)
{
  return this->numAPinDiffArea_;
}

int 
defiPin_numAPinPartialCutArea(const defiPin * this)
{
  return this->numAPinPartialCutArea_;
}

int 
defiPin_APinPartialMetalArea(const defiPin * this,
			     int i)
{
  return this->APinPartialMetalArea_[i];
}

int 
defiPin_hasAPinPartialMetalAreaLayer(const defiPin * this,
				     int i)
{
  return (this->APinPartialMetalAreaLayer_[i] && *(this->APinPartialMetalAreaLayer_[i])) ? 1 : 0;
}

const char *
defiPin_APinPartialMetalAreaLayer(const defiPin * this,
				  int i)
{
  return this->APinPartialMetalAreaLayer_[i];
}

int 
defiPin_APinPartialMetalSideArea(const defiPin * this,
				 int i)
{
  return this->APinPartialMetalSideArea_[i];
}

int 
defiPin_hasAPinPartialMetalSideAreaLayer(const defiPin * this,
					 int i)
{
  return (this->APinPartialMetalSideAreaLayer_[i] && *(this->APinPartialMetalSideAreaLayer_[i])) ? 1 : 0;
}

const char *
defiPin_APinPartialMetalSideAreaLayer(const defiPin * this,
				      int i)
{
  return this->APinPartialMetalSideAreaLayer_[i];
}

int 
defiPin_APinDiffArea(const defiPin * this,
		     int i)
{
  return this->APinDiffArea_[i];
}

int 
defiPin_hasAPinDiffAreaLayer(const defiPin * this,
			     int i)
{
  return (this->APinDiffAreaLayer_[i] && *(this->APinDiffAreaLayer_[i])) ? 1 : 0;
}

const char *
defiPin_APinDiffAreaLayer(const defiPin * this,
			  int i)
{
  return this->APinDiffAreaLayer_[i];
}

int 
defiPin_APinPartialCutArea(const defiPin * this,
			   int i)
{
  return this->APinPartialCutArea_[i];
}

int 
defiPin_hasAPinPartialCutAreaLayer(const defiPin * this,
				   int i)
{
  return (this->APinPartialCutAreaLayer_[i] && *(this->APinPartialCutAreaLayer_[i])) ? 1 : 0;
}

const char *
defiPin_APinPartialCutAreaLayer(const defiPin * this,
				int i)
{
  return this->APinPartialCutAreaLayer_[i];
}

/*
 * 5.6
 */ void 
defiPin_addPolygon(defiPin * this,
		   const char *layerName)
{
  int    *pms, *pdw;

  int     i;

  if (this->numPolys_ == this->polysAllocated_) {
    char  **newn;

    struct defiPoints **poly;

    this->polysAllocated_ = (this->polysAllocated_ == 0) ? 2 : this->polysAllocated_ * 2;
    newn = (char **) defMalloc(sizeof(char *) * this->polysAllocated_);
    poly = (struct defiPoints **) defMalloc(sizeof(struct defiPoints *) * this->polysAllocated_);
    pms = (int *) defMalloc(this->polysAllocated_ * sizeof(int));
    pdw = (int *) defMalloc(this->polysAllocated_ * sizeof(int));
    for (i = 0;
	 i < this->numPolys_;
	 i++) {
      newn[i] = this->polygonNames_[i];
      poly[i] = this->polygons_[i];
      pms[i] = this->polyMinSpacing_[i];
      pdw[i] = this->polyEffectiveWidth_[i];
    }
    if (this->numPolys_ > 0) {
      defFree((char *) (this->polygons_));
      defFree((char *) (this->polygonNames_));
      defFree((char *) (this->polyMinSpacing_));
      defFree((char *) (this->polyEffectiveWidth_));
    }
    this->polygonNames_ = newn;
    this->polygons_ = poly;
    this->polyMinSpacing_ = pms;
    this->polyEffectiveWidth_ = pdw;
  }
  this->polygonNames_[this->numPolys_] = strdup(layerName);
  this->polygons_[this->numPolys_] = 0;
  this->polyMinSpacing_[this->numPolys_] = -1;
  this->polyEffectiveWidth_[this->numPolys_] = -1;
  this->numPolys_ += 1;
}

/*
 * 5.6
 */ void 
defiPin_addPolygonPts(defiPin * this,
		      defiGeometries * geom)
{
  struct defiPoints *p;

  int     x, y;

  int     i;

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
  this->polygons_[this->numPolys_ - 1] = p;
}

/*
 * 5.6
 */ void 
defiPin_addPolySpacing(defiPin * this,
		       int minSpacing)
{
  this->polyMinSpacing_[this->numPolys_ - 1] = minSpacing;
}

/*
 * 5.6
 */ void 
defiPin_addPolyDesignRuleWidth(defiPin * this,
			       int effectiveWidth)
{
  this->polyEffectiveWidth_[this->numPolys_ - 1] = effectiveWidth;
}

/*
 * 5.6
 */ int 
defiPin_numPolygons(const defiPin * this)
{
  return this->numPolys_;
}

/*
 * 5.6
 */ const char *
defiPin_polygonName(const defiPin * this,
		    int index)
{
  if (index < 0 || index > this->numPolys_) {
    defiError("index out of bounds");
    return 0;
  }
  return this->polygonNames_[index];
}

/*
 * 5.6
 */ struct defiPoints 
defiPin_getPolygon(const defiPin * this,
		   int index)
{
  return *(this->polygons_[index]);
}

/*
 * 5.6
 */ int 
defiPin_hasPolygonSpacing(const defiPin * this,
			  int index)
{
  if (this->polyMinSpacing_[index] == -1)
    return 0;
  return 1;
}

/*
 * 5.6
 */ int 
defiPin_hasPolygonDesignRuleWidth(const defiPin * this,
				  int index)
{
  if (this->polyEffectiveWidth_[index] == -1)
    return 0;
  return 1;
}

/*
 * 5.6
 */ int 
defiPin_polygonSpacing(const defiPin * this,
		       int index)
{
  return this->polyMinSpacing_[index];
}

/*
 * 5.6
 */ int 
defiPin_polygonDesignRuleWidth(const defiPin * this,
			       int index)
{
  return this->polyEffectiveWidth_[index];
}

/*
 * 5.6
 */ int 
defiPin_hasNetExpr(const defiPin * this)
{
  return (int) (this->hasNetExpr_);
}

/*
 * 5.6
 */ const char *
defiPin_netExpr(const defiPin * this)
{
  return this->netExpr_;
}

/*
 * 5.6
 */ int 
defiPin_hasSupplySensitivity(const defiPin * this)
{
  return (int) (this->hasSupplySens_);
}

/*
 * 5.6
 */ const char *
defiPin_supplySensitivity(const defiPin * this)
{
  return this->supplySens_;
}

/*
 * 5.6
 */ int 
defiPin_hasGroundSensitivity(const defiPin * this)
{
  return (int) (this->hasGroundSens_);
}

/*
 * 5.6
 */ const char *
defiPin_groundSensitivity(const defiPin * this)
{
  return this->groundSens_;
}

/*
 * 5.7
 */ void 
defiPin_addVia(defiPin * this,
	       const char *viaName,
	       int ptX,
	       int ptY)
{
  if (this->numVias_ >= this->viasAllocated_) {
    int     i;

    char  **newl;

    int    *nx, *ny;

    this->viasAllocated_ = this->viasAllocated_ ? this->viasAllocated_ * 2 : 8;
    newl = (char **) defMalloc(this->viasAllocated_ * sizeof(char *));
    nx = (int *) defMalloc(this->viasAllocated_ * sizeof(int));
    ny = (int *) defMalloc(this->viasAllocated_ * sizeof(int));
    for (i = 0;
	 i < this->numVias_;
	 i++) {
      newl[i] = this->viaNames_[i];
      nx[i] = this->viaX_[i];
      ny[i] = this->viaY_[i];
    }
    if (this->numVias_ > 0) {
      defFree((char *) this->viaNames_);
      defFree((char *) this->viaX_);
      defFree((char *) this->viaY_);
    }
    this->viaNames_ = newl;
    this->viaX_ = nx;
    this->viaY_ = ny;
  }
  this->viaNames_[this->numVias_] = (char *) defMalloc(strlen(viaName) + 1);
  strcpy(this->viaNames_[this->numVias_], DEFCASE(viaName));
  this->viaX_[this->numVias_] = ptX;
  this->viaY_[this->numVias_] = ptY;
  this->numVias_ += 1;
}

/*
 * 5.7
 */ int 
defiPin_numVias(const defiPin * this)
{
  return this->numVias_;
}

/*
 * 5.7
 */ const char *
defiPin_viaName(const defiPin * this,
		int index)
{
  if (index < 0 || index > this->numVias_) {
    defiError("index out of bounds");
    return 0;
  }
  return this->viaNames_[index];
}

/*
 * 5.7
 */ int 
defiPin_viaPtX(const defiPin * this,
	       int index)
{
  return this->viaX_[index];
}

/*
 * 5.7
 */ int 
defiPin_viaPtY(const defiPin * this,
	       int index)
{
  return this->viaY_[index];
}

/*
 * 5.7
 */ void 
defiPin_addPort(defiPin * this)
{
  defiPinPort **pp;

  defiPinPort *pv;

  int     i;

  if (this->numPorts_ >= this->portsAllocated_) {
    if (this->portsAllocated_ == 0) {
      this->pinPort_ = (defiPinPort **) defMalloc(sizeof(defiPinPort *) * 4);
      this->portsAllocated_ = 4;
    }
    else {
      this->portsAllocated_ = this->portsAllocated_ * 2;
      pp = (defiPinPort **) defMalloc(sizeof(defiPinPort *) * this->portsAllocated_);
      for (i = 0;
	   i < this->numPorts_;
	   i++)
	pp[i] = this->pinPort_[i];
      defFree((char *) (this->pinPort_));
      this->pinPort_ = pp;
    }
  }
  pv = (defiPinPort *) defMalloc(sizeof(defiPinPort));
  defiPinPort_Init(pv);
  this->pinPort_[this->numPorts_] = pv;
  this->numPorts_ += 1;
}

/*
 * 5.7
 */ void 
defiPin_addPortLayer(defiPin * this,
		     const char *layer)
{
  int     i = this->numPorts_ - 1;

  defiPinPort_addLayer(this->pinPort_[i],
		       layer);
}

/*
 * 5.7
 */ void 
defiPin_addPortLayerSpacing(defiPin * this,
			    int minSpacing)
{
  int     i = this->numPorts_ - 1;

  defiPinPort_addLayerSpacing(this->pinPort_[i],
			      minSpacing);
}

/*
 * 5.7
 */ void 
defiPin_addPortLayerDesignRuleWidth(defiPin * this,
				    int effectiveWidth)
{
  int     i = this->numPorts_ - 1;

  defiPinPort_addLayerDesignRuleWidth(this->pinPort_[i],
				      effectiveWidth);
}

/*
 * 5.7
 */ void 
defiPin_addPortLayerPts(defiPin * this,
			int xl,
			int yl,
			int xh,
			int yh)
{
  int     i = this->numPorts_ - 1;

  defiPinPort_addLayerPts(this->pinPort_[i],
			  xl,
			  yl,
			  xh,
			  yh);
}

/*
 * 5.7
 */ void 
defiPin_addPortPolygon(defiPin * this,
		       const char *layerName)
{
  int     i = this->numPorts_ - 1;

  defiPinPort_addPolygon(this->pinPort_[i],
			 layerName);
}

/*
 * 5.7
 */ void 
defiPin_addPortPolySpacing(defiPin * this,
			   int minSpacing)
{
  int     i = this->numPorts_ - 1;

  defiPinPort_addPolySpacing(this->pinPort_[i],
			     minSpacing);
}

/*
 * 5.7
 */ void 
defiPin_addPortPolyDesignRuleWidth(defiPin * this,
				   int effectiveWidth)
{
  int     i = this->numPorts_ - 1;

  defiPinPort_addPolyDesignRuleWidth(this->pinPort_[i],
				     effectiveWidth);
}

/*
 * 5.7
 */ void 
defiPin_addPortPolygonPts(defiPin * this,
			  defiGeometries * geom)
{
  int     i = this->numPorts_ - 1;

  defiPinPort_addPolygonPts(this->pinPort_[i],
			    geom);
}

/*
 * 5.7
 */ void 
defiPin_addPortVia(defiPin * this,
		   const char *via,
		   int viaX,
		   int viaY)
{
  int     i = this->numPorts_ - 1;

  defiPinPort_addVia(this->pinPort_[i],
		     via,
		     viaX,
		     viaY);
}

/*
 * 5.7
 */ void 
defiPin_setPortPlacement(defiPin * this,
			 int typ,
			 int x,
			 int y,
			 int orient)
{
  int     i = this->numPorts_ - 1;

  defiPinPort_setPlacement(this->pinPort_[i],
			   typ,
			   x,
			   y,
			   orient);
}

/*
 * 5.7
 */ int 
defiPin_hasPort(const defiPin * this)
{
  return this->numPorts_;
}

/*
 * 5.7
 */ int 
defiPin_numPorts(const defiPin * this)
{
  return this->numPorts_;
}

/*
 * 5.7
 */ defiPinPort *
defiPin_pinPort(const defiPin * this,
		int index)
{
  if (index < 0 || index > this->numPorts_) {
    defiError("index out of bounds");
    return 0;
  }
  return this->pinPort_[index];
}

void 
defiPin_print(const defiPin * this,
	      FILE * f)
{
  int     xl, yl, xh, yh;

  int     i;

  fprintf(f, "PINS '%s' on net '%s'\n", defiPin_pinName(this), defiPin_netName(this));
  if (defiPin_hasDirection(this))
    fprintf(f, "+ DIRECTION '%s'\n", defiPin_direction(this));
  if (defiPin_hasNetExpr(this))
    fprintf(f, "+ NETEXPR '%s'\n", defiPin_netExpr(this));
  if (defiPin_hasSupplySensitivity(this))
    fprintf(f, "+ SUPPLYSENSITIVITY '%s'\n", defiPin_supplySensitivity(this));
  if (defiPin_hasGroundSensitivity(this))
    fprintf(f, "+ GROUNDSENSITIVITY '%s'\n", defiPin_groundSensitivity(this));
  if (defiPin_hasUse(this))
    fprintf(f, "+ USE '%s'\n", defiPin_use(this));
  if (defiPin_hasLayer(this)) {
    for (i = 0;
	 i < defiPin_numLayer(this);
	 i++) {
      defiPin_bounds(this,
		     i,
		     &xl,
		     &yl,
		     &xh,
		     &yh);
      fprintf(f, "+ LAYER '%s' %d %d %d %d\n", defiPin_layer(this,
							i), xl, yl, xh, yh);
    }
  }
  for (i = 0;
       i < defiPin_numPolygons(this);
       i++) {
    fprintf(f, "+ POLYGON %s", defiPin_polygonName(this,
						   i));
    if (defiPin_hasPolygonSpacing(this,
				  i))
      fprintf(f, " SPACING %g", defiPin_polygonSpacing(this,
						       i));
    if (defiPin_hasPolygonDesignRuleWidth(this,
					  i))
      fprintf(f, " DESIGNRULEWIDTH %g", defiPin_polygonDesignRuleWidth(this,
								       i));
  }
  for (i = 0;
       i < defiPin_numVias(this);
       i++) {
    fprintf(f, "+ VIA %s %g %g\n", defiPin_viaName(this,
						   i), defiPin_viaPtX(this,
						    i), defiPin_viaPtY(this,
								       i));
  }
  if (defiPin_hasPlacement(this))
    fprintf(f, " PLACED %s%s%d %d\n", defiPin_isFixed(this) ? " FIXED" : "", defiPin_isCover(this) ? " COVER" : "", defiPin_placementX(this), defiPin_placementY(this));
  if (defiPin_hasSpecial(this))
    fprintf(f, "+ SPECIAL\n");
}
