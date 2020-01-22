// (The MIT License)

// Copyright (c) 2012 Thomas Blobaum <tblobaum@gmail.com>

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// 'Software'), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <v8.h>
#include <node.h>
#include <syslog.h>
#include <string>

using namespace std;
using namespace node;
using namespace v8;

char title[1024];

void open(const FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  v8::String::Utf8Value t(isolate, args[0]);
  strcpy(title, *t);

  int facility = args[1].As<Number>()->Value();
  int log_upto = args[2].As<Number>()->Value();

  setlogmask(LOG_UPTO(log_upto));
  openlog(title, LOG_PID | LOG_NDELAY, facility);


  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "true").ToLocalChecked());
}

void exit(const FunctionCallbackInfo<v8::Value>& args) {
  closelog();

  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "true").ToLocalChecked());
}

void log(const FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  int severity = args[0].As<Number>()->Value();

  v8::String::Utf8Value message(isolate, args[1]);
  syslog(severity, "%s", *message );

  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "true").ToLocalChecked());
}

void init(v8::Local<v8::Object> target) {
  NODE_SET_METHOD(target, "open", open);
  NODE_SET_METHOD(target, "exit", exit);
  NODE_SET_METHOD(target, "log", log);
}

NODE_MODULE(syslog, init)
