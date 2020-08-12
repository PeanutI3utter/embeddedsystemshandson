#ifndef SAMPLEFILTER_H_
#define SAMPLEFILTER_H_
#define SAMPLEFILTER_TAP_NUM 13

typedef struct {
  double history[SAMPLEFILTER_TAP_NUM];
  unsigned int last_index;
  float OFFSET;
} SampleFilter;

void SampleFilter_init(SampleFilter *f);
void SampleFilter_put(SampleFilter *f, double input);
double SampleFilter_get(SampleFilter *f);

#endif
