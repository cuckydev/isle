#ifndef MXMUSICPRESENTER_H
#define MXMUSICPRESENTER_H

#include "mxaudiopresenter.h"

// VTABLE 0x100dc9b8
class MxMusicPresenter : public MxAudioPresenter
{
public:
  // OFFSET: LEGO1 0x100c23a0
  inline virtual const char *ClassName() const override // vtable+0xc
  {
    // 0x10101e48
    return "MxMusicPresenter";
  }

  // OFFSET: LEGO1 0x100c23b0
  inline virtual MxBool IsA(const char *name) const override // vtable+0x10
  {
    return !strcmp(name, MxMusicPresenter::ClassName()) || MxAudioPresenter::IsA(name);
  }

  MxMusicPresenter();

private:
  void Init();
};

#endif // MXMUSICPRESENTER_H
