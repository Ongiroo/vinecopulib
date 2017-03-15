// Copyright © 2017 Thomas Nagler and Thibault Vatter
//
// This file is part of the vinecopulib library and licensed under the terms of
// the MIT license. For a copy, see the LICENSE file in the root directory of
// vinecopulib or https://tvatter.github.io/vinecopulib/.

#include "bicop_archimedean.hpp"

namespace vinecopulib
{
    Eigen::VectorXd ArchimedeanBicop::pdf_default(const Eigen::MatrixXd& u)
    {
        Eigen::VectorXd f = Eigen::VectorXd::Ones(u.rows());
        Eigen::VectorXd v = generator(u.col(0)) + generator(u.col(1));
        f = generator_derivative(u.col(0)).cwiseProduct(generator_derivative(u.col(1)));
        Eigen::VectorXd numerator = generator_derivative2(generator_inv(v));
        Eigen::VectorXd denominator = generator_derivative(generator_inv(v)).array().pow(3.0);
        f = (-1)*f.cwiseProduct(numerator);
        f = f.cwiseQuotient(denominator);

        return f;
    }

    Eigen::VectorXd ArchimedeanBicop::hfunc1_default(const Eigen::MatrixXd& u)
    {
        Eigen::VectorXd h(u.rows());
        Eigen::VectorXd v(u.rows());

        v = generator(u.col(0)) + generator(u.col(1));
        h = generator_derivative(u.col(0)).cwiseQuotient(generator_derivative(generator_inv(v)));

        return h;
    }

    Eigen::VectorXd ArchimedeanBicop::hfunc2_default(const Eigen::MatrixXd& u)
    {
        return hfunc1_default(swap_cols(u));
    }

    Eigen::VectorXd ArchimedeanBicop::hinv1_default(const Eigen::MatrixXd& u)
    {
        Eigen::VectorXd hinv = hinv1_num(u);
        return hinv;
    }

    Eigen::VectorXd ArchimedeanBicop::hinv2_default(const Eigen::MatrixXd& u)
    {
        return hinv1_default(swap_cols(u));
    }

    void ArchimedeanBicop::flip()
    {
        if (rotation_ == 90) {
            set_rotation(270);
        } else if (rotation_ == 270) {
            set_rotation(90);
        }
    }

    Eigen::VectorXd ArchimedeanBicop::get_start_parameters(const double tau)
    {
        Eigen::MatrixXd bounds = this->get_parameters_bounds();
        Eigen::VectorXd parameters = bounds.col(0) + Eigen::VectorXd::Constant(2, 0.1);
        return parameters;
    }
}
