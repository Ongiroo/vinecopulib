// Copyright © 2017 Thomas Nagler and Thibault Vatter
//
// This file is part of the vinecopulib library and licensed under the terms of
// the MIT license. For a copy, see the LICENSE file in the root directory of
// vinecopulib or https://vinecopulib.github.io/vinecopulib/.

#pragma once

#include <memory>
#include <vector>

#include <vinecopulib/misc/tools_eigen.hpp>
#include <vinecopulib/bicop/family.hpp>

namespace vinecopulib {
//! @brief An abstract class for bivariate copula families
//!
//! This class is used in the implementation underlying the Bicop class.
//! Users should not use AbstractBicop or derived classes directly, but
//! always work with the Bicop interface.
class AbstractBicop
{
    friend class Bicop;

public:
    virtual ~AbstractBicop() = 0;

protected:
    // Factories
    static std::shared_ptr <AbstractBicop> create(
        BicopFamily family = BicopFamily::indep,
        const Eigen::MatrixXd &parameters = Eigen::MatrixXd());

    // Getters and setters
    BicopFamily get_family() const;

    std::string get_family_name() const;

    virtual Eigen::MatrixXd get_parameters() const = 0;

    virtual void set_parameters(const Eigen::MatrixXd &parameters) = 0;

    virtual void flip() = 0;

    // Virtual methods
    virtual void fit(const Eigen::Matrix<double, Eigen::Dynamic, 2> &data,
                     std::string method, double mult) = 0;

    virtual double calculate_npars() = 0;

    virtual double parameters_to_tau(const Eigen::MatrixXd &parameters) = 0;

    virtual Eigen::VectorXd pdf(
        const Eigen::Matrix<double, Eigen::Dynamic, 2> &u) = 0;

    virtual Eigen::VectorXd cdf(
        const Eigen::Matrix<double, Eigen::Dynamic, 2> &u) = 0;

    virtual Eigen::VectorXd hfunc1(
        const Eigen::Matrix<double, Eigen::Dynamic, 2> &u) = 0;

    virtual Eigen::VectorXd hfunc2(
        const Eigen::Matrix<double, Eigen::Dynamic, 2> &u) = 0;

    virtual Eigen::VectorXd hinv1(
        const Eigen::Matrix<double, Eigen::Dynamic, 2> &u) = 0;

    virtual Eigen::VectorXd hinv2(
        const Eigen::Matrix<double, Eigen::Dynamic, 2> &u) = 0;

    virtual Eigen::MatrixXd tau_to_parameters(const double &tau) = 0;

    // Misc methods
    Eigen::VectorXd hinv1_num(
        const Eigen::Matrix<double, Eigen::Dynamic, 2> &u);

    Eigen::VectorXd hinv2_num(
        const Eigen::Matrix<double, Eigen::Dynamic, 2> &u);

    // Data members
    BicopFamily family_;
};

//! A shared pointer to an object of class AbstracBicop.
typedef std::shared_ptr <AbstractBicop> BicopPtr;

Eigen::VectorXd no_tau_to_parameters(const double &);
}

#include <vinecopulib/bicop/implementation/abstract.ipp>
