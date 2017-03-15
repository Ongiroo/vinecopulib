// Copyright © 2017 Thomas Nagler and Thibault Vatter
//
// This file is part of the vinecopulib library and licensed under the terms of
// the MIT license. For a copy, see the LICENSE file in the root directory of
// vinecopulib or https://tvatter.github.io/vinecopulib/.


#include "bicop_clayton.hpp"

namespace vinecopulib
{
    ClaytonBicop::ClaytonBicop()
    {
        family_ = 3;
        family_name_ = "Clayton";
        rotation_ = 0;
        association_direction_ = "positive";
        parameters_ = VectorXd::Zero(1);
        parameters_bounds_ = MatrixXd::Zero(1, 2);
        parameters_bounds_(0, 1) = 200.0;
    }

    ClaytonBicop::ClaytonBicop(const VectorXd& parameters)
    {
        ClaytonBicop();
        set_parameters(parameters);
    }

    ClaytonBicop::ClaytonBicop(const VectorXd& parameters, const int& rotation)
    {
        ClaytonBicop();
        set_parameters(parameters);
        set_rotation(rotation);
    }

    VectorXd ClaytonBicop::generator(const VectorXd& u)
    {
        double theta = double(this->parameters_(0));
        auto f = [theta](const double v) {
            return (std::pow(v, -theta)-1)/theta;
        };
        return u.unaryExpr(f);
    }
    VectorXd ClaytonBicop::generator_inv(const VectorXd& u)
    {
        double theta = double(this->parameters_(0));
        auto f = [theta](const double v) {
            return std::pow(1+theta*v, -1/theta);
        };
        return u.unaryExpr(f);
    }

    VectorXd ClaytonBicop::generator_derivative(const VectorXd& u)
    {
        double theta = double(this->parameters_(0));
        auto f = [theta](const double v) {
            return (-1)*std::pow(v, -1-theta);
        };
        return u.unaryExpr(f);
    }

    VectorXd ClaytonBicop::generator_derivative2(const VectorXd& u)
    {
        double theta = double(this->parameters_(0));
        auto f = [theta](const double v) {
            return (1+theta)*std::pow(v, -2-theta);
        };
        return u.unaryExpr(f);
    }

    VectorXd ClaytonBicop::hinv1_default(const MatrixXd& u)
    {
        double theta = double(this->parameters_(0));
        VectorXd hinv = u.col(0).array().pow(theta + 1.0);
        if (theta < 75) {
            hinv = u.col(1).cwiseProduct(hinv);
            hinv = hinv.array().pow(-theta/(theta + 1.0));
            VectorXd x = u.col(0);
            x = x.array().pow(-theta);
            hinv = hinv - x + VectorXd::Ones(x.size());
            hinv = hinv.array().pow(-1/theta);
        } else {
            hinv = hinv1_num(u);
        }
        return hinv;
    }

    VectorXd ClaytonBicop::tau_to_parameters(const double& tau)
    {
        VectorXd parameters(1);
        parameters(0) = 2 * std::fabs(tau) / (1 - std::fabs(tau));
        return parameters;
    }

    double ClaytonBicop::parameters_to_tau(const VectorXd& parameters)
    {
        double tau =  parameters(0) / (2 + std::fabs(parameters(0)));
        if ((rotation_ == 90) | (rotation_ == 270))
            tau *= -1;
        return tau;
    }

    VectorXd ClaytonBicop::get_start_parameters(const double tau)
    {
        return tau_to_parameters(tau);
    }
}

/*// PDF
VectorXd ClaytonBicop::pdf_default(const MatrixXd& u)
{
    double theta = double(this->parameters_(0));
    VectorXd t1 = generator(u.col(0));
    VectorXd t2 = generator(u.col(1));
    VectorXd t = t1+t2;
    VectorXd f = generator_inv(t);

    t1 = u.col(0).array().pow(theta);
    t2 = u.col(1).array().pow(theta);
    t = t1 + t2 - t1.cwiseProduct(t2);
    t = t.array().square();

    t1 = t1.array().pow((theta-1)/theta);
    t2 = t2.array().pow((theta-1)/theta);

    f = (1+theta)*f.cwiseQuotient(t).cwiseProduct(t1).cwiseProduct(t2);
    return f;
}

// hfunction
VectorXd ClaytonBicop::hfunc1_default(const MatrixXd& u)
{
    double theta = double(this->parameters_(0));
    VectorXd t1 = generator(u.col(0));
    VectorXd t2 = generator(u.col(1));
    VectorXd t = t1+t2;
    VectorXd f = generator_inv(t);
    f = f.array().pow(1+theta);

    t2 = u.col(0).array().pow(-1-theta);
    f = f.cwiseProduct(t2);
    return f;
}*/
