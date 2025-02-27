
#include "mxmatrix.h"

#include <memory.h>
#include "math.h"

#include "decomp.h"

DECOMP_SIZE_ASSERT(MxMatrix, 0x8);
DECOMP_SIZE_ASSERT(MxMatrixData, 0x48);

// OFFSET: LEGO1 0x10002340
void MxMatrix::EqualsMxMatrix(const MxMatrix *p_other)
{
  memcpy(m_data, p_other->m_data, 16 * sizeof(float));
}

// OFFSET: LEGO1 0x10002320
void MxMatrix::EqualsMatrixData(const float *p_matrix)
{
  memcpy(m_data, p_matrix, 16 * sizeof(float));
}

// OFFSET: LEGO1 0x10002370
void MxMatrix::SetData(float *p_data)
{
  m_data = p_data;
}

// OFFSET: LEGO1 0x10002360
void MxMatrix::AnotherSetData(float *p_data)
{
  m_data = p_data;
}

// OFFSET: LEGO1 0x10002390
float *MxMatrix::GetData()
{
  return m_data;
}

// OFFSET: LEGO1 0x10002380
const float *MxMatrix::GetData() const
{
  return m_data;
}

// OFFSET: LEGO1 0x100023c0
float *MxMatrix::Element(int p_row, int p_col)
{
  return &m_data[p_row * 4 + p_col];
}

// OFFSET: LEGO1 0x100023a0
const float *MxMatrix::Element(int p_row, int p_col) const
{
  return &m_data[p_row * 4 + p_col];
}

// OFFSET: LEGO1 0x100023e0
void MxMatrix::Clear()
{
  memset(m_data, 0, 16 * sizeof(float));
}

// OFFSET: LEGO1 0x100023f0
void MxMatrix::SetIdentity()
{
  Clear();
  m_data[0] = 1.0f;
  m_data[5] = 1.0f;
  m_data[10] = 1.0f;
  m_data[15] = 1.0f;
}

// OFFSET: LEGO1 0x10002850
void MxMatrix::operator=(const MxMatrix& p_other)
{
  EqualsMxMatrix(&p_other);
}

// OFFSET: LEGO1 0x10002430
MxMatrix* MxMatrix::operator+=(const float *p_matrix)
{
  for (int i = 0; i < 16; ++i)
    m_data[i] += p_matrix[i];
  return this;
}

// Matches but instructions are significantly out of order. Probably not wrong
// code given that the very similar SetTranslation does match.
// OFFSET: LEGO1 0x10002460
void MxMatrix::TranslateBy(const float *p_x, const float *p_y, const float *p_z)
{
  m_data[12] += *p_x;
  m_data[13] += *p_y;
  m_data[14] += *p_z;
}

// OFFSET: LEGO1 0x100024a0
void MxMatrix::SetTranslation(const float *p_x, const float *p_y, const float *p_z)
{
  m_data[12] = *p_x;
  m_data[13] = *p_y;
  m_data[14] = *p_z;
}

// OFFSET: LEGO1 0x10002530
void MxMatrix::EqualsMxProduct(const MxMatrix *p_a, const MxMatrix *p_b)
{
  EqualsDataProduct(p_a->m_data, p_b->m_data);
}

// OFFSET: LEGO1 0x100024d0
void MxMatrix::EqualsDataProduct(const float *p_a, const float *p_b)
{
  float *cur = m_data;
  for (int row = 0; row < 4; ++row)
  {
    for (int col = 0; col < 4; ++col)
    {
      *cur = 0.0f;
      for (int k = 0; k < 4; ++k)
      {
        *cur += p_a[row * 4 + k] * p_b[k * 4 + col];
      }
      cur++;
    }
  }
}

// Not close, Ghidra struggles understinging this method so it will have to
// be manually worked out. Included since I at least figured out what it was
// doing with rotateIndex and what overall operation it's trying to do.
// OFFSET: LEGO1 0x10002550 STUB
void MxMatrix::ToQuaternion(MxVector4 *p_outQuat)
{
  float trace = m_data[0] + m_data[5] + m_data[10];
  if (trace > 0)
  {
    trace = sqrt(trace + 1.0);
    p_outQuat->GetData()[3] = trace * 0.5f;
    p_outQuat->GetData()[0] = (m_data[9] - m_data[6]) * trace;
    p_outQuat->GetData()[1] = (m_data[2] - m_data[8]) * trace;
    p_outQuat->GetData()[2] = (m_data[4] - m_data[1]) * trace;
    return;
  }

  // OFFSET: LEGO1 0x100d4090
  static int rotateIndex[] = {1, 2, 0};

  // Largest element along the trace
  int largest = m_data[0] < m_data[5];
  if (*Element(largest, largest) < m_data[10])
    largest = 2;

  int next = rotateIndex[largest];
  int nextNext = rotateIndex[next];
  float valueA = *Element(nextNext, nextNext);
  float valueB = *Element(next, next);
  float valueC = *Element(largest, largest);

  // Above is somewhat decomped, below is pure speculation since the automatic
  // decomp becomes very garbled.
  float traceValue = sqrt(valueA - valueB - valueC + 1.0);

  p_outQuat->GetData()[largest] = traceValue * 0.5f;
  traceValue = 0.5f / traceValue;

  p_outQuat->GetData()[3] = (m_data[next + 4 * nextNext] - m_data[nextNext + 4 * next]) * traceValue;
  p_outQuat->GetData()[next] = (m_data[next + 4 * largest] + m_data[largest + 4 * next]) * traceValue;
  p_outQuat->GetData()[nextNext] = (m_data[nextNext + 4 * largest] + m_data[largest + 4 * nextNext]) * traceValue;
}

// No idea what this function is doing and it will be hard to tell until
// we have a confirmed usage site.
// OFFSET: LEGO1 0x10002710 STUB
MxResult MxMatrix::DoSomethingWithLength(const MxVector3 *p_vec)
{
  return FAILURE;
}

// OFFSET: LEGO1 0x10002860
void MxMatrixData::operator=(const MxMatrixData& p_other)
{
  EqualsMxMatrix(&p_other);
}
