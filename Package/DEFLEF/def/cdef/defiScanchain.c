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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lex.h"
#include "defiScanchain.h"
#include "defiDebug.h"
void 
defiOrdered_clear(defiOrdered * this)
{
  int     i;

  for (i = 0;
       i < this->num_;
       i++) {
    defFree((char *) (this->inst_[i]));
    if (this->in_[i])
      defFree((char *) (this->in_[i]));
    if (this->out_[i])
      defFree((char *) (this->out_[i]));
  }
  this->num_ = 0;
}

void 
defiOrdered_Destroy(defiOrdered * this)
{
  int     i;

  for (i = 0;
       i < this->num_;
       i++) {
    defFree((char *) (this->inst_[i]));
    defFree((char *) (this->in_[i]));
    defFree((char *) (this->out_[i]));
  }
  defFree((char *) (this->inst_));
  defFree((char *) (this->in_));
  defFree((char *) (this->out_));
  defFree((char *) (this->bits_));
}

void 
defiOrdered_Init(defiOrdered * this)
{
  this->num_ = 0;
  this->allocated_ = 32;
  this->inst_ = (char **) defMalloc(sizeof(char *) * 32);
  this->in_ = (char **) defMalloc(sizeof(char *) * 32);
  this->out_ = (char **) defMalloc(sizeof(char *) * 32);
  this->bits_ = (int *) defMalloc(sizeof(int) * 32);
}

void 
defiOrdered_bump(defiOrdered * this)
{
  int     max = this->allocated_ * 2;

  int     i;

  char  **nin = (char **) defMalloc(sizeof(char *) * max);

  char  **nout = (char **) defMalloc(sizeof(char *) * max);

  char  **ninst = (char **) defMalloc(sizeof(char *) * max);

  int    *nbits = (int *) defMalloc(sizeof(int) * max);

  for (i = 0;
       i < this->num_;
       i++) {
    nin[i] = this->in_[i];
    nout[i] = this->out_[i];
    ninst[i] = this->inst_[i];
    nbits[i] = this->bits_[i];
  }
  defFree((char *) (this->inst_));
  defFree((char *) (this->in_));
  defFree((char *) (this->out_));
  defFree((char *) (this->bits_));
  this->allocated_ = max;
  this->inst_ = ninst;
  this->in_ = nin;
  this->out_ = nout;
  this->bits_ = nbits;
}

void 
defiOrdered_addOrdered(defiOrdered * this,
		       const char *inst)
{
  if (this->num_ == this->allocated_)
    defiOrdered_bump(this);
  this->inst_[this->num_] = (char *) defMalloc(strlen(inst) + 1);
  strcpy(this->inst_[this->num_], DEFCASE(inst));
  this->in_[this->num_] = 0;
  this->out_[this->num_] = 0;
  this->bits_[this->num_] = -1;
  this->num_ += 1;
}

void 
defiOrdered_addIn(defiOrdered * this,
		  const char *pin)
{
  this->in_[this->num_ - 1] = (char *) defMalloc(strlen(pin) + 1);
  strcpy(this->in_[this->num_ - 1], DEFCASE(pin));
}

void 
defiOrdered_addOut(defiOrdered * this,
		   const char *pin)
{
  this->out_[this->num_ - 1] = (char *) defMalloc(strlen(pin) + 1);
  strcpy(this->out_[this->num_ - 1], DEFCASE(pin));
}

void 
defiOrdered_setOrderedBits(defiOrdered * this,
			   int bits)
{
  this->bits_[this->num_ - 1] = bits;
}

int 
defiOrdered_num(const defiOrdered * this)
{
  return this->num_;
}

char  **
defiOrdered_inst(const defiOrdered * this)
{
  return this->inst_;
}

char  **
defiOrdered_in(const defiOrdered * this)
{
  return this->in_;
}

char  **
defiOrdered_out(const defiOrdered * this)
{
  return this->out_;
}

int    *
defiOrdered_bits(const defiOrdered * this)
{
  return this->bits_;
}

defiScanchain *
defiScanchain_CreateUninitialized()
{
  defiScanchain *this = (defiScanchain *) malloc(sizeof(defiScanchain));

  defiScanchain_Init(this);
  return (this);
}

void 
defiScanchain_Init(defiScanchain * this)
{
  this->name_ = (char *) defMalloc(32);
  this->nameLength_ = 32;
  this->numOrdered_ = 0;
  this->numOrderedAllocated_ = 4;
  this->ordered_ = (defiOrdered **) defMalloc(sizeof(defiOrdered *) * 4);
  this->numFloating_ = 0;
  this->numFloatingAllocated_ = 4;
  this->floatInst_ = (char **) defMalloc(sizeof(char *) * 4);
  this->floatIn_ = (char **) defMalloc(sizeof(char *) * 4);
  this->floatOut_ = (char **) defMalloc(sizeof(char *) * 4);
  this->floatBits_ = (int *) defMalloc(sizeof(int) * 4);
  this->stopInst_ = 0;
  this->stopPin_ = 0;
  this->startInst_ = 0;
  this->startPin_ = 0;
  this->hasStart_ = 0;
  this->hasStop_ = 0;
  this->commonInPin_ = 0;
  this->commonOutPin_ = 0;
  this->hasPartition_ = 0;
  this->partName_ = 0;
  this->maxBits_ = -1;
}

void 
defiScanchain_clear(defiScanchain * this)
{
  int     i;

  defiOrdered *o;

  for (i = 0;
       i < this->numOrdered_;
       i++) {
    o = this->ordered_[i];
    defiOrdered_Destroy(o);
    defFree((char *) o);
    this->ordered_[i] = 0;
  }
  this->numOrdered_ = 0;
  for (i = 0;
       i < this->numFloating_;
       i++) {
    if (this->floatIn_[i])
      defFree(this->floatIn_[i]);
    if (this->floatOut_[i])
      defFree(this->floatOut_[i]);
    defFree(this->floatInst_[i]);
    this->floatInst_[i] = 0;
    this->floatBits_[i] = -1;
  }
  this->numFloating_ = 0;
  if (this->stopInst_)
    defFree(this->stopInst_);
  if (this->stopPin_)
    defFree(this->stopPin_);
  if (this->startInst_)
    defFree(this->startInst_);
  if (this->startPin_)
    defFree(this->startPin_);
  if (this->commonInPin_)
    defFree(this->commonInPin_);
  if (this->commonOutPin_)
    defFree(this->commonOutPin_);
  this->stopInst_ = 0;
  this->stopPin_ = 0;
  this->startInst_ = 0;
  this->startPin_ = 0;
  this->hasStart_ = 0;
  this->hasStop_ = 0;
  this->commonInPin_ = 0;
  this->commonOutPin_ = 0;
  this->hasPartition_ = 0;
  if (this->partName_)
    defFree((char *) (this->partName_));
  this->partName_ = 0;
  this->maxBits_ = -1;
}

void 
defiScanchain_Destroy(defiScanchain * this)
{
  defiScanchain_clear(this);
  defFree(this->name_);
  defFree((char *) (this->ordered_));
  defFree((char *) (this->floatInst_));
  defFree((char *) (this->floatIn_));
  defFree((char *) (this->floatOut_));
  defFree((char *) (this->floatBits_));
}

void
defiScanchain_Delete(defiScanchain * this)
{
  if (this == NULL)
    return;
  defiScanchain_Destroy(this);
  free(this);
}

void 
defiScanchain_setName(defiScanchain * this,
		      const char *name)
{
  int     len = strlen(name) + 1;

  defiScanchain_clear(this);
  if (len > this->nameLength_) {
    defFree(this->name_);
    this->name_ = (char *) defMalloc(len);
    this->nameLength_ = len;
  }
  strcpy(this->name_, DEFCASE(name));
}

const char *
defiScanchain_name(const defiScanchain * this)
{
  return this->name_;
}

void 
defiScanchain_addFloatingInst(defiScanchain * this,
			      const char *name)
{
  if (this->numFloating_ >= this->numFloatingAllocated_) {
    int     max = 2 * this->numFloatingAllocated_;

    int     i;

    char  **ninst = (char **) defMalloc(sizeof(char *) * max);

    char  **nin = (char **) defMalloc(sizeof(char *) * max);

    char  **nout = (char **) defMalloc(sizeof(char *) * max);

    int    *nbits = (int *) defMalloc(sizeof(int) * max);

    for (i = 0;
	 i < this->numFloating_;
	 i++) {
      ninst[i] = this->floatInst_[i];
      nin[i] = this->floatIn_[i];
      nout[i] = this->floatOut_[i];
      nbits[i] = this->floatBits_[i];
    }
    defFree((char *) (this->floatInst_));
    defFree((char *) (this->floatIn_));
    defFree((char *) (this->floatOut_));
    defFree((char *) (this->floatBits_));
    this->floatInst_ = ninst;
    this->floatOut_ = nout;
    this->floatIn_ = nin;
    this->floatBits_ = nbits;
    this->numFloatingAllocated_ = max;
  }
  this->floatInst_[this->numFloating_] = (char *) defMalloc(strlen(name) + 1);
  strcpy(this->floatInst_[this->numFloating_], DEFCASE(name));
  this->floatIn_[this->numFloating_] = 0;
  this->floatOut_[this->numFloating_] = 0;
  this->floatBits_[this->numFloating_] = -1;
  this->numFloating_ += 1;
}

void 
defiScanchain_addFloatingIn(defiScanchain * this,
			    const char *name)
{
  int     len = strlen(name) + 1;

  this->floatIn_[this->numFloating_ - 1] = (char *) defMalloc(len);
  strcpy(this->floatIn_[this->numFloating_ - 1], DEFCASE(name));
}

void 
defiScanchain_addFloatingOut(defiScanchain * this,
			     const char *name)
{
  int     len = strlen(name) + 1;

  this->floatOut_[this->numFloating_ - 1] = (char *) defMalloc(len);
  strcpy(this->floatOut_[this->numFloating_ - 1], DEFCASE(name));
}

void 
defiScanchain_setFloatingBits(defiScanchain * this,
			      int bits)
{
  this->floatBits_[this->numFloating_ - 1] = bits;
}

void 
defiScanchain_addOrderedIn(defiScanchain * this,
			   const char *name)
{
  defiOrdered *o = this->ordered_[this->numOrdered_ - 1];

  defiOrdered_addIn(o,
		    name);
}

void 
defiScanchain_addOrderedOut(defiScanchain * this,
			    const char *name)
{
  defiOrdered *o = this->ordered_[this->numOrdered_ - 1];

  defiOrdered_addOut(o,
		     name);
}

void 
defiScanchain_addOrderedInst(defiScanchain * this,
			     const char *name)
{
  defiOrdered *o = this->ordered_[this->numOrdered_ - 1];

  defiOrdered_addOrdered(o,
			 name);
}

void 
defiScanchain_setOrderedBits(defiScanchain * this,
			     int bits)
{
  defiOrdered *o = this->ordered_[this->numOrdered_ - 1];

  defiOrdered_setOrderedBits(o,
			     bits);
}

void 
defiScanchain_addOrderedList(defiScanchain * this)
{
  defiOrdered *o;

  if (this->numOrdered_ == this->numOrderedAllocated_) {
    int     max = 2 * this->numOrderedAllocated_;

    int     i;

    defiOrdered **no = (defiOrdered **) defMalloc(sizeof(defiOrdered *) * max);

    for (i = 0;
	 i < this->numOrdered_;
	 i++) {
      no[i] = this->ordered_[i];
    }
    defFree((char *) (this->ordered_));
    this->ordered_ = no;
    this->numOrderedAllocated_ = max;
  }
  o = (defiOrdered *) defMalloc(sizeof(defiOrdered));
  this->ordered_[this->numOrdered_] = o;
  defiOrdered_Init(o);
  this->numOrdered_ += 1;
}

void 
defiScanchain_setStart(defiScanchain * this,
		       const char *inst,
		       const char *pin)
{
  int     len;

  if (this->startInst_)
    defiError("ERROR (DEFPARS-6150): SCANCHAINS START statement is defined multiple times.\nThis is illegal.");
  len = strlen(inst) + 1;
  this->startInst_ = (char *) defMalloc(len);
  strcpy(this->startInst_, DEFCASE(inst));
  len = strlen(pin) + 1;
  this->startPin_ = (char *) defMalloc(len);
  strcpy(this->startPin_, DEFCASE(pin));
  this->hasStart_ = 1;
}

void 
defiScanchain_setStop(defiScanchain * this,
		      const char *inst,
		      const char *pin)
{
  int     len;

  if (this->stopInst_)
    defiError("ERROR (DEFPARS-6151): SCANCHAINS STOP statement is defined multiple times.\nThis is illegal.");
  len = strlen(inst) + 1;
  this->stopInst_ = (char *) defMalloc(len);
  strcpy(this->stopInst_, DEFCASE(inst));
  len = strlen(pin) + 1;
  this->stopPin_ = (char *) defMalloc(len);
  strcpy(this->stopPin_, DEFCASE(pin));
  this->hasStop_ = 1;
}

/*
 * 5.4.1
 */ void 
defiScanchain_setPartition(defiScanchain * this,
			   const char *partName,
			   int maxBits)
{
  if (this->partName_)
    defFree(this->partName_);
  this->partName_ = (char *) defMalloc(strlen(partName) + 1);
  strcpy(this->partName_, DEFCASE(partName));
  this->maxBits_ = maxBits;
  this->hasPartition_ = 1;
}

int 
defiScanchain_hasStart(const defiScanchain * this)
{
  return (int) (this->hasStart_);
}

int 
defiScanchain_hasStop(const defiScanchain * this)
{
  return (int) (this->hasStop_);
}

int 
defiScanchain_hasFloating(const defiScanchain * this)
{
  return this->numFloating_ ? 1 : 0;
}

int 
defiScanchain_hasOrdered(const defiScanchain * this)
{
  return this->numOrdered_ ? 1 : 0;
}

/*
 * 5.4.1
 */ int 
defiScanchain_hasPartition(const defiScanchain * this)
{
  return this->hasPartition_;
}

/*
 * 5.4.1
 */ int 
defiScanchain_hasPartitionMaxBits(const defiScanchain * this)
{
  return (this->maxBits_ != -1) ? 1 : 0;
}

/*
 * 5.4.1
 */ const char *
defiScanchain_partitionName(const defiScanchain * this)
{
  return this->partName_;
}

/*
 * 5.4.1
 */ int 
defiScanchain_partitionMaxBits(const defiScanchain * this)
{
  return this->maxBits_;
}

void 
defiScanchain_start(const defiScanchain * this,
		    char **inst,
		    char **pin)
{
  if (inst)
    *inst = this->startInst_;
  if (pin)
    *pin = this->startPin_;
}

void 
defiScanchain_stop(const defiScanchain * this,
		   char **inst,
		   char **pin)
{
  if (inst)
    *inst = this->stopInst_;
  if (pin)
    *pin = this->stopPin_;
}

int 
defiScanchain_numOrderedLists(const defiScanchain * this)
{
  return this->numOrdered_;
}

void 
defiScanchain_ordered(const defiScanchain * this,
		      int index,
		      int *size,
		      char ***inst,
		      char ***inPin,
		      char ***outPin,
		      int **bits)
{
  defiOrdered *o;

  if (index >= 0 && index <= this->numOrdered_) {
    o = this->ordered_[index];
    *size = defiOrdered_num(o);
    *inst = defiOrdered_inst(o);
    *inPin = defiOrdered_in(o);
    *outPin = defiOrdered_out(o);
    *bits = defiOrdered_bits(o);
  }
  else {
    *size = 10;
    *inst = 0;
  }
}

void 
defiScanchain_floating(const defiScanchain * this,
		       int *size,
		       char ***inst,
		       char ***inPin,
		       char ***outPin,
		       int **bits)
{
  *size = this->numFloating_;
  *inst = this->floatInst_;
  *inPin = this->floatIn_;
  *outPin = this->floatOut_;
  *bits = this->floatBits_;
}

void 
defiScanchain_setCommonOut(defiScanchain * this,
			   const char *pin)
{
  int     len = strlen(pin) + 1;

  this->commonOutPin_ = (char *) defMalloc(len);
  strcpy(this->commonOutPin_, DEFCASE(pin));
}

void 
defiScanchain_setCommonIn(defiScanchain * this,
			  const char *pin)
{
  int     len = strlen(pin) + 1;

  this->commonInPin_ = (char *) defMalloc(len);
  strcpy(this->commonInPin_, DEFCASE(pin));
}

int 
defiScanchain_hasCommonInPin(const defiScanchain * this)
{
  return this->commonInPin_ ? 1 : 0;
}

int 
defiScanchain_hasCommonOutPin(const defiScanchain * this)
{
  return this->commonOutPin_ ? 1 : 0;
}

const char *
defiScanchain_commonInPin(const defiScanchain * this)
{
  return this->commonInPin_;
}

const char *
defiScanchain_commonOutPin(const defiScanchain * this)
{
  return this->commonOutPin_;
}

void 
defiScanchain_print(const defiScanchain * this,
		    FILE * f)
{
  char   *a;

  char   *b;

  char  **inst;

  char  **in;

  char  **out;

  int    *bits;

  int     num;

  int     i;

  int     h;

  fprintf(f, "Scan chain '%s' %d things\n", defiScanchain_name(this), defiScanchain_hasStart(this) + defiScanchain_hasStop(this) + this->numFloating_ + defiScanchain_numOrderedLists(this));
  if (defiScanchain_hasStart(this)) {
    defiScanchain_start(this,
			&a,
			&b);
    fprintf(f, " start inst '%s' pin '%s'\n", a, b);
  }
  if (defiScanchain_hasStop(this)) {
    defiScanchain_stop(this,
		       &a,
		       &b);
    fprintf(f, " stop inst '%s' pin '%s'\n", a, b);
  }
  defiScanchain_floating(this,
			 &num,
			 &inst,
			 &in,
			 &out,
			 &bits);
  for (i = 0;
       i < num;
       i++) {
    fprintf(f, " floating '%s' IN=%s OUT=%s BITS=%d\n", inst[i], in[i] ? in[i] : "NIL", out[i] ? out[i] : "NIL", bits[i] ? bits[i] : 0);
  }
  for (i = 0;
       i < defiScanchain_numOrderedLists(this);
       i++) {
    defiScanchain_ordered(this,
			  i,
			  &num,
			  &inst,
			  &in,
			  &out,
			  &bits);
    fprintf(f, " %d ordered\n", i);
    for (h = 0;
	 h < num;
	 h++) {
      fprintf(f, " '%s' IN=%s OUT=%s BITS=%d\n", inst[h], in[h] ? in[h] : "NIL", out[h] ? out[h] : "NIL", bits[h] ? bits[h] : 0);
    }
  }
  if (defiScanchain_hasCommonInPin(this))
    fprintf(f, " common in pin %s\n", defiScanchain_commonInPin(this));
  if (defiScanchain_hasCommonOutPin(this))
    fprintf(f, " common out pin %s\n", defiScanchain_commonOutPin(this));
}
