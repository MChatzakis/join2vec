/**
 * Copyright (c) 2016-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cstdint>
#include <ostream>
#include <vector>
#include <iostream>

#include "real.h"

#include <math.h>

namespace fasttext {

class Matrix;

class Vector {
 protected:
  std::vector<real> data_;
  std::vector<int> r_data_;

 public:
  explicit Vector(int64_t);
  Vector(const Vector&) = default;
  Vector(Vector&&) noexcept = default;
  Vector& operator=(const Vector&) = default;
  Vector& operator=(Vector&&) = default;

  inline real* data() {
    return data_.data();
  }

  inline const real* data() const {
    return data_.data();
  }

  inline real& operator[](int64_t i) {
    return data_[i];
  }

  inline const real& operator[](int64_t i) const {
    return data_[i];
  }

  inline int64_t size() const {
    return data_.size();
  }

  void keep_dims(int64_t i) {
    data_.resize(i);
    return;
  }

  void set_decimals(int64_t i){
    int decimals = i;

    for(int i=0; i<data_.size(); i++){
      data_[i] = floor(pow(10, decimals) * data_[i]) / pow(10, decimals);
    }
  }

  void round_vals(){
    for(int i=0; i<data_.size(); i++){
      r_data_.push_back(round(data_[i]));
      //r_data_.push_back(ceil(data_[i]));
    }
    return;
  }

  int *r_data(){
    return r_data_.data();
  }
  

  void zero();
  void mul(real);
  real norm() const;
  void addVector(const Vector& source);
  void addVector(const Vector&, real);
  void addRow(const Matrix&, int64_t);
  void addRow(const Matrix&, int64_t, real);
  void mul(const Matrix&, const Vector&);
  int64_t argmax();
};

std::ostream& operator<<(std::ostream&, const Vector&);

} // namespace fasttext
