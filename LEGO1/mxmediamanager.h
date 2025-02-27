#ifndef MXMEDIAMANGER_H
#define MXMEDIAMANGER_H

#include "mxcore.h"
#include "mxcriticalsection.h"
#include "mxthread.h"
#include "mxpresenterlist.h"
#include "mxtypes.h"

// VTABLE 0x100dc6b0
// SIZE 0x2c
class MxMediaManager : public MxCore
{
public:
  MxMediaManager();
  virtual ~MxMediaManager() override;

  virtual MxResult Tickle(); // vtable+08
  virtual MxResult InitPresenters(); // vtable+14
  virtual void Destroy(); // vtable+18
  virtual void AddPresenter(MxPresenter &p_presenter); // vtable+1c
  virtual void RemovePresenter(MxPresenter &p_presenter); // vtable+20
  virtual void StopPresenters(); // vtable+24

  MxResult Init();

protected:
  MxPresenterList *m_presenters;
  MxThread *m_thread; // 0xc
  MxCriticalSection m_criticalSection; // 0x10
};

#endif // MXMEDIAMANGER_H
