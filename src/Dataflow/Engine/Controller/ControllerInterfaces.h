/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2020 Scientific Computing and Imaging Institute,
   University of Utah.

   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/


/// @todo Documentation Dataflow/Engine/Controller/ControllerInterfaces.h

#ifndef ENGINE_NETWORK_CONTROLLERINTERFACES_H
#define ENGINE_NETWORK_CONTROLLERINTERFACES_H

#include <Dataflow/Network/NetworkFwd.h>
#include <Dataflow/Engine/Controller/share.h>

namespace SCIRun {
namespace Dataflow {
namespace Engine {

  template <class Memento>
  class SCISHARE NetworkIOInterface
  {
  public:
    virtual ~NetworkIOInterface() {}
    virtual Memento saveNetwork() const = 0;
    virtual void loadNetwork(const Memento& xml) = 0;
    virtual void clear() = 0;
  };

  typedef boost::shared_ptr<NetworkIOInterface<Networks::NetworkFileHandle>> NetworkIOHandle;

  template <class Memento>
  class ProvenanceItem;

  typedef boost::shared_ptr<Dataflow::Engine::ProvenanceItem<Networks::NetworkFileHandle> > ProvenanceItemHandle;

  template <class Memento>
  class ProvenanceManager;

  typedef boost::shared_ptr<Dataflow::Engine::ProvenanceManager<Networks::NetworkFileHandle> > ProvenanceManagerHandle;
}
}
}

#endif
