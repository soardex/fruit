// expect-success
/*
 * Copyright 2014 Google Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fruit/fruit.h>
#include "test_macros.h"

using fruit::Component;
using fruit::Injector;

struct Annotation1 {};
struct Annotation2 {};

struct Y {
  using Inject = fruit::Annotated<Annotation1, Y>();
  Y() {
    Assert(!constructed);
    constructed = true;
  }
  
  static bool constructed;
};

using YAnnot = fruit::Annotated<Annotation1, Y>;

bool Y::constructed = false;

struct X {
  INJECT(X(ANNOTATED(Annotation1, fruit::Provider<Y>) provider)) : provider(provider) {
    Assert(!constructed);
    constructed = true;
  }
  
  void run() {
    Y* y(provider);
    (void) y;
  }
  
  fruit::Provider<Y> provider;
  
  static bool constructed;
};

bool X::constructed = false;

fruit::Component<X> getComponent() {
  return fruit::createComponent();
}

int main() {
  
  fruit::NormalizedComponent<> normalizedComponent(fruit::createComponent());
  Injector<X> injector(normalizedComponent, getComponent());
  
  Assert(!X::constructed);
  Assert(!Y::constructed);
  
  X* x(injector);
  
  Assert(X::constructed);
  Assert(!Y::constructed);
  
  x->run();
  
  Assert(X::constructed);
  Assert(Y::constructed);
  
  return 0;
}
