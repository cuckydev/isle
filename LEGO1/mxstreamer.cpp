#include "mxstreamer.h"

#include <algorithm>

#include "legoomni.h"
#include "mxdiskstreamcontroller.h"
#include "mxramstreamcontroller.h"

DECOMP_SIZE_ASSERT(MxStreamer, 0x2c);

#define MXSTREAMER_DELETE_NOTIFY 6

// OFFSET: LEGO1 0x100b8f00
MxStreamer::MxStreamer()
{
  NotificationManager()->Register(this);
}

// OFFSET: LEGO1 0x100b9190
MxResult MxStreamer::Init()
{
  undefined *b = new undefined[m_subclass1.GetSize() * 0x5800];
  m_subclass1.SetBuffer(b);
  if (b) {
    b = new undefined[m_subclass2.GetSize() * 0x800];
    m_subclass2.SetBuffer(b);
    if (b) {
      return SUCCESS;
    }
  }

  return FAILURE;
}

// OFFSET: LEGO1 0x100b91d0
MxStreamer::~MxStreamer()
{
  while (!m_openStreams.empty()) {
    MxStreamController *c = m_openStreams.front();
    m_openStreams.pop_front();
    delete c;
  }

  NotificationManager()->Unregister(this);
}

// OFFSET: LEGO1 0x100b92c0
MxStreamController *MxStreamer::Open(const char *p_name, MxU16 p_lookupType)
{
  // TODO
  MxStreamController *stream = NULL;

  if (!GetOpenStream(p_name)) {
    switch (p_lookupType) {
    case e_DiskStream:
      stream = new MxDiskStreamController();
      break;
    case e_RAMStream:
      stream = new MxRAMStreamController();
      break;
    }

    if (!stream
      || stream->Open(p_name) != SUCCESS
      || AddStreamControllerToOpenList(stream) != SUCCESS) {
      delete stream;
      stream = NULL;
    }
  }

  return stream;
}

// OFFSET: LEGO1 0x100b9570
MxLong MxStreamer::Close(const char *p)
{
  MxDSAction ds;

  ds.SetUnknown24(-2);

  for (list<MxStreamController *>::iterator it = m_openStreams.begin(); it != m_openStreams.end(); it++) {
    MxStreamController *c = *it;

    if (!p || !strcmp(p, c->atom.GetInternal())) {
      m_openStreams.erase(it);

      if (!c->FUN_100c20d0(ds)) {
        MxStreamerNotification notif(MXSTREAMER_DELETE_NOTIFY, NULL, c);

        NotificationManager()->Send(this, &notif);
      } else {
        delete c;
      }

      return SUCCESS;
    }
  }

  return FAILURE;
}

// OFFSET: LEGO1 0x100b9700
MxParam *MxStreamerNotification::Clone()
{
  return new MxStreamerNotification(m_type, m_sender, m_controller);
}

// OFFSET: LEGO1 0x100b9870
MxStreamController *MxStreamer::GetOpenStream(const char *p_name)
{
  for (list<MxStreamController *>::iterator it = m_openStreams.begin(); it != m_openStreams.end(); it++) {
    MxStreamController *c = *it;
    MxAtomId &atom = c->atom;
    if (p_name) {
      if (!strcmp(atom.GetInternal(), p_name)) {
       return *it;
      }
    }
  }

  return NULL;
}


// OFFSET: LEGO1 0x100b9930
MxResult MxStreamer::AddStreamControllerToOpenList(MxStreamController *stream)
{
  if (find(m_openStreams.begin(), m_openStreams.end(), stream) == m_openStreams.end()) {
    m_openStreams.push_back(stream);
    return SUCCESS;
  }

  return FAILURE;
}

// OFFSET: LEGO1 0x100b9b60
MxLong MxStreamer::Notify(MxParam &p)
{
  if (p.GetType() == MXSTREAMER_DELETE_NOTIFY) {
    MxDSAction ds;

    ds.SetUnknown24(-2);

    MxStreamController *c = static_cast<MxStreamerNotification&>(p).GetController();

    if (!c->FUN_100c20d0(ds)) {
      MxStreamerNotification notif(MXSTREAMER_DELETE_NOTIFY, NULL, c);
      NotificationManager()->Send(this, &notif);
    } else {
      delete c;
    }
  }

  return 0;
}

// No offset, function is always inlined
MxStreamerSubClass1::MxStreamerSubClass1(undefined4 size)
{
  m_buffer = NULL;
  m_size = size;
  undefined4 *ptr = &m_unk08;
  for (int i = 0; i >= 0; i--) {
    ptr[i] = 0;
  }
}
