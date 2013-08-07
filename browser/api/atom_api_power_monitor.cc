// Copyright (c) 2013 GitHub, Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "browser/api/atom_api_power_monitor.h"

#include "base/power_monitor/power_monitor.h"

namespace atom {

namespace api {

PowerMonitor::PowerMonitor(v8::Handle<v8::Object> wrapper)
    : EventEmitter(wrapper) {
  base::PowerMonitor::Get()->AddObserver(this);
}

PowerMonitor::~PowerMonitor() {
  base::PowerMonitor::Get()->RemoveObserver(this);
}

void PowerMonitor::OnPowerStateChange(bool on_battery_power) {
  if (on_battery_power)
    Emit("on-battery");
  else
    Emit("on-ac");
}

void PowerMonitor::OnSuspend() {
  Emit("suspend");
}

void PowerMonitor::OnResume() {
  Emit("resume");
}

// static
v8::Handle<v8::Value> PowerMonitor::New(const v8::Arguments& args) {
  v8::HandleScope scope;

  if (!args.IsConstructCall())
    return node::ThrowError("Require constructor call");

  new PowerMonitor(args.This());

  return args.This();
}

// static
void PowerMonitor::Initialize(v8::Handle<v8::Object> target) {
  v8::HandleScope scope;

  v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(
      PowerMonitor::New);
  t->InstanceTemplate()->SetInternalFieldCount(1);
  t->SetClassName(v8::String::NewSymbol("PowerMonitor"));

  target->Set(v8::String::NewSymbol("PowerMonitor"), t->GetFunction());
}


}  // namespace api

}  // namespace atom

NODE_MODULE(atom_browser_power_monitor, atom::api::PowerMonitor::Initialize)