#include "FIRFilter.hpp"

static double filter_taps[SAMPLEFILTER_TAP_NUM] = {
    -0.039290291314124086, 0.032633951815758636,  -0.046843775384870355,
    0.08479881677397579,   -0.07226629762533697,  0.08134707034887309,
    0.8869924265570199,    0.08134707034887309,   -0.07226629762533697,
    0.08479881677397579,   -0.046843775384870355, 0.032633951815758636,
    -0.039290291314124086};

void SampleFilter_init(SampleFilter *f) {
  int i;
  for (i = 0; i < SAMPLEFILTER_TAP_NUM; ++i)
    f->history[i] = 0;
  f->last_index = 0;
}

void SampleFilter_put(SampleFilter *f, double input) {
  input += f->OFFSET;

  f->history[f->last_index++] = input;
  if (f->last_index == SAMPLEFILTER_TAP_NUM)
    f->last_index = 0;
}

double SampleFilter_get(SampleFilter *f) {
  double acc = 0;
  int index = f->last_index, i;
  for (i = 0; i < SAMPLEFILTER_TAP_NUM; ++i) {
    index = index != 0 ? index - 1 : SAMPLEFILTER_TAP_NUM - 1;
    acc += f->history[index] * filter_taps[i];
  };
  return acc;
}