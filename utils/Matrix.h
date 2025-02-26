#pragma once

#include "LinearCongruence.h"
#include <assert.h>
#include <cstdio>
#include <iostream>
#include <optional>
#include <type_traits>
#include <vector>

template <typename T>
std::vector<T> operator%(const std::vector<T> &vec, int m)
{
  std::vector<T> result(vec.size());

  for (size_t i = 0; i < vec.size(); i++)
  {
    result[i] = (vec[i] % m + m) % m;
  }

  return result;
}

namespace ModMatrix
{

  template <typename T>
  struct Matrix
  {

    explicit Matrix<T>() : cols(2), rows(2)
    {
      data.resize(2, std::vector<T>(2, 0));
    }

    explicit Matrix<T>(int _rows, int _cols) : rows(_rows), cols(_cols)
    {
      data.resize(_rows, std::vector<T>(_cols, 0));
    }

    explicit Matrix(const std::vector<T> &values)
    {
      if (values.size() < 4 || values.size() % 2 != 0)
      {
        throw std::invalid_argument("Matrix constructor requires at least 4 "
                                    "elements and an even number.");
      }

      rows = values.size() / 2;
      cols = 2;

      data.resize(rows, std::vector<T>(cols));

      for (int i = 0; i < rows; i++)
      {
        std::copy(values.begin() + (i * cols), values.begin() + ((i + 1) * cols),
                  data[i].begin());
      }
    }

    Matrix<T> operator*(int n)
    {
      Matrix<T> result;

      for (int i = 0; i < data.size(); i++)
      {
        for (int j = 0; j < data[0].size(); j++)
        {
          result[i][j] = LinearAlg::Mod(data[i][j] * n, 26);
        }
      }
      return result;
    }

    Matrix<T> operator*(const Matrix<T> &other)
    {
      std::cout << "using matrix *operator" << "\n";
      int rows = data.size();
      int cols = other.data[0].size();

      int common = data[0].size();

      assert(cols == other.data.size());

      Matrix<T> result(rows, cols);

      for (int i = 0; i < rows; i++)
      {
        for (int j = 0; j < cols; j++)
        {
          for (int k = 0; k < common; k++)
          {
            result.data[i][j] += data[i][k] * other.data[k][j];
          }
        }
      }

      return result % 26;
    }

    Matrix<T> operator%(int m)
    {
      Matrix<T> result(data.size(), data[0].size());

      for (int i = 0; i < data.size(); i++)
      {
        for (int j = 0; j < data[i].size(); j++)
        {
          result[i][j] = LinearAlg::Mod(data[i][j], m);
        }
      }

      return result;
    }

    std::vector<int> &operator[](int index) { return data[index]; }

    const std::vector<int> &operator[](int index) const { return data[index]; }

    T Determinant()
    {
      assert(data.size() == 2 && data[0].size() == 2);
      T value = (data[0][0] * data[1][1]) - (data[0][1] * data[1][0]);
      return value;
    }

    Matrix<T> scalarMultiply(const Matrix<T> &mat, T scalar, int m = 26)
    {
      Matrix<T> result = mat; // Copy matrix

      for (size_t i = 0; i < mat.data.size(); i++)
      {
        for (size_t j = 0; j < mat.data[0].size(); j++)
        {
          result.data[i][j] = LinearAlg::Mod(scalar * mat.data[i][j], m);
        }
      }
      return result;
    }

    std::optional<Matrix<T>> Inverse(int m)
    {

      int det = Determinant();

      int detMod = LinearAlg::Mod(det, m);

      if (detMod == 0)
        return std::nullopt;

      std::cout << "determinant: " << det << std::endl;
      std::cout << "Modded det: " << detMod << std::endl;

      std::optional<int> detInv = LinearAlg::MultiplicativeInverse(detMod, m);

      if (!detInv.has_value())
      {
        printf("Determinant has no inverse with 26\n");
        return std::nullopt;
      }
      std::cout << "Inverse determinant: " << detInv.value() << std::endl;

      Matrix<T> adj(
          {data[1][1], -data[0][1],
           -data[1][0], data[0][0]});

      std::cout << "------- ADJ ------ " << std::endl;
      adj.Print();
      Matrix<T> result = scalarMultiply(adj, detInv.value());
      result = result % 26;

      return result % 26;
    }

    void Print()
    {

      for (int i = 0; i < data.size(); i++)
      {
        for (int j = 0; j < data[i].size(); j++)
        {
          std::cout << data[i][j] << std::endl;
        }
      }
    }

    int cols;
    int rows;

    std::vector<std::vector<T>> data;
  };

} // namespace ModMatrix

template <typename T>
std::vector<T> operator*(const ModMatrix::Matrix<T> &mat, const std::vector<T> &vec)
{
  assert(vec.size() == 2);

  std::vector<T> result(2, 0);

  result[0] = LinearAlg::Mod(mat.data[0][0] * vec[0] + mat.data[0][1] * vec[1], 26);
  result[1] = LinearAlg::Mod(mat.data[1][0] * vec[0] + mat.data[1][1] * vec[1], 26);

  return result;
}

template <typename T>
std::vector<T> operator*(const std::vector<T> &vec, const ModMatrix::Matrix<T> &mat)
{
  assert(vec.size() == 2);
  std::vector<T> result(2, 0);

  /// (a  b) * [(a , b), (c, d)] -> [(aa + bc), (ab, bd)]

  int aa = vec[0] * mat.data[0][0];
  int bc = vec[1] * mat.data[1][0];
  result[0] = LinearAlg::Mod(aa + bc, 26);
  
  int ab = LinearAlg::Mod(vec[0] * mat.data[0][1], 26);
  int bd = LinearAlg::Mod(vec[1] * mat.data[1][1], 26);
  result[1] = LinearAlg::Mod(ab + bd, 26);

  return result;
}
