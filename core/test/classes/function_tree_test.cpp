//This file is part of Bertini 2.0.
//
//function_tree_test.cpp is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//function_tree_test.cpp is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with function_tree_test.cpp.  If not, see <http://www.gnu.org/licenses/>.
//

//  function_tree_test.cpp
//
//  Created by Collins, James B. on 4/30/15.
//  Copyright (c) 2015 West Texas A&M University. All rights reserved.
//
// also modified by
//  Daniel Brake
//  University of Notre Dame
//  ACMS
//  Spring, Summer 2015

#include <iostream>

#include <cstdlib>
#include <cmath>

#include "bertini.hpp"
#include "function_tree.hpp"


#include <boost/spirit/include/qi.hpp>
#include <boost/test/unit_test.hpp>



using Variable = bertini::node::Variable;
using Node = bertini::node::Node;
using Float = bertini::node::Float;
using Function = bertini::node::Function;


double relaxed_threshold_clearance_d = 1e-14;
double threshold_clearance_d = 1e-15;

unsigned FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS = 30;
double threshold_clearance_mp = 1e-27;

std::string xstr_real = "3.1";
std::string xstr_imag = "4.1";
std::string ystr_real = "8.8";
std::string ystr_imag = "9.9";
std::string astr_real = "3.4";
std::string astr_imag = "5.6";
std::string bstr_real = "-0.2";
std::string bstr_imag = "-2.1";
std::string pstr_real = "0.8";
std::string pstr_imag = "-1.7";


dbl xnum_dbl(std::stod(xstr_real), std::stod(xstr_imag));
dbl ynum_dbl(std::stod(ystr_real), std::stod(ystr_imag));
dbl anum_dbl(std::stod(astr_real), std::stod(astr_imag));
dbl bnum_dbl(std::stod(bstr_real), std::stod(bstr_imag));
dbl pnum_dbl(std::stod(pstr_real), std::stod(pstr_imag));

mpfr xnum_mpfr(xstr_real, xstr_imag);
mpfr ynum_mpfr(ystr_real, ystr_imag);
mpfr anum_mpfr(astr_real, astr_imag);
mpfr bnum_mpfr(bstr_real, bstr_imag);
mpfr pnum_mpfr(pstr_real, pstr_imag);


BOOST_AUTO_TEST_SUITE(function_tree_class)

/////////// Basic Operations Alone ///////////////////

BOOST_AUTO_TEST_CASE(manual_construction_num_squared){
	using mpfr_float = boost::multiprecision::mpfr_float;
	
	
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	
	dbl exact_dbl = anum_dbl*anum_dbl;
	mpfr exact_mpfr = anum_mpfr*anum_mpfr;
	
	std::shared_ptr<Node> N = a;
	
	N *= N;
	BOOST_CHECK_EQUAL(N->Degree(),0);
	BOOST_CHECK(N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag()) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
}


BOOST_AUTO_TEST_CASE(manual_construction_x_squared){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	
	dbl exact_dbl = xnum_dbl*xnum_dbl;
	mpfr exact_mpfr = xnum_mpfr*xnum_mpfr;
	
	std::shared_ptr<Node> N = x;
	
	N *= N;
	
	x->set_current_value<dbl>(std::complex<double>(xnum_dbl));
	x->set_current_value<mpfr>(bertini::complex(xnum_mpfr));
	
	BOOST_CHECK(N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag()) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
	BOOST_CHECK_EQUAL(N->Degree(),2);
	BOOST_CHECK_EQUAL(N->Degree(x),2);
	

}


BOOST_AUTO_TEST_CASE(manual_construction_sqrt_x){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	
	dbl exact_dbl = sqrt(xnum_dbl);
	mpfr exact_mpfr = sqrt(xnum_mpfr);
	
	std::shared_ptr<Node> N = pow(x, 1.0/2);
	
	x->set_current_value<dbl>(std::complex<double>(xnum_dbl));
	x->set_current_value<mpfr>(bertini::complex(xnum_mpfr));
	
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);

	BOOST_CHECK(! N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag()) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_x_plus_y_plus_number){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Variable> y = std::make_shared<Variable>("y");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	y->set_current_value<dbl>(ynum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	y->set_current_value<mpfr>(bertini::complex(ystr_real,ystr_imag));
	
	dbl exact_dbl = xnum_dbl+ynum_dbl+anum_dbl;
	mpfr exact_mpfr = xnum_mpfr+ynum_mpfr+anum_mpfr;
	
	std::shared_ptr<Node> N = x+y+a;
	
	BOOST_CHECK_EQUAL(N->Degree(),1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag()) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
	N = a+x+y;
	BOOST_CHECK_EQUAL(N->Degree(),1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag()) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
	N = y+a+x;
	BOOST_CHECK_EQUAL(N->Degree(),1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag()) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
	
	N = y+x+a;
	BOOST_CHECK_EQUAL(N->Degree(),1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag()) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
	
	N = x+a+y;
	BOOST_CHECK_EQUAL(N->Degree(),1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
	N = a+y+x;
	BOOST_CHECK_EQUAL(N->Degree(),1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag()) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
}


BOOST_AUTO_TEST_CASE(manual_construction_x_minus_y_minus_number){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Variable> y = std::make_shared<Variable>("y");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	y->set_current_value<dbl>(ynum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	y->set_current_value<mpfr>(bertini::complex(ystr_real,ystr_imag));
	
	dbl exact_dbl = xnum_dbl-ynum_dbl-anum_dbl;
	mpfr exact_mpfr = xnum_mpfr-ynum_mpfr-anum_mpfr;
	
	std::shared_ptr<Node> N = x-y-a;
	BOOST_CHECK_EQUAL(N->Degree(),1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
	N = x-a-y;
	BOOST_CHECK_EQUAL(N->Degree(),1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);


	N = -a-y+x;
	BOOST_CHECK_EQUAL(N->Degree(),1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);


	N = -a+x-y;
	BOOST_CHECK_EQUAL(N->Degree(),1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);

	N = -y-a+x;
	BOOST_CHECK_EQUAL(N->Degree(),1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);


	N = -y+x-a;
	BOOST_CHECK_EQUAL(N->Degree(),1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);


	
}


BOOST_AUTO_TEST_CASE(manual_construction_x_times_y_times_number){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Variable> y = std::make_shared<Variable>("y");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	y->set_current_value<dbl>(ynum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	y->set_current_value<mpfr>(bertini::complex(ystr_real,ystr_imag));
	
	dbl exact_dbl = xnum_dbl*ynum_dbl*anum_dbl;
	mpfr exact_mpfr = xnum_mpfr*ynum_mpfr*anum_mpfr;
	
	
	std::shared_ptr<Node> N = x*y*a;
	BOOST_CHECK_EQUAL(N->Degree(),2);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() / exact_dbl.real() -1) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() / exact_dbl.imag() -1) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() / exact_mpfr.real() -1) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() / exact_mpfr.imag() -1) < threshold_clearance_mp);
	
	N = a*x*y;
	BOOST_CHECK_EQUAL(N->Degree(),2);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() / exact_dbl.real() -1) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() / exact_dbl.imag() -1) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() / exact_mpfr.real() -1) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() / exact_mpfr.imag() -1) < threshold_clearance_mp);
	
	N = y*a*x;
	BOOST_CHECK_EQUAL(N->Degree(),2);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() / exact_dbl.real() -1) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() / exact_dbl.imag() -1) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() / exact_mpfr.real() -1) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() / exact_mpfr.imag() -1) < threshold_clearance_mp);
	
	
	N = y*x*a;
	BOOST_CHECK_EQUAL(N->Degree(),2);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() / exact_dbl.real() -1) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() / exact_dbl.imag() -1) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() / exact_mpfr.real() -1) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() / exact_mpfr.imag() -1) < threshold_clearance_mp);
	
	
	N = x*a*y;
	BOOST_CHECK_EQUAL(N->Degree(),2);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() / exact_dbl.real() -1) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() / exact_dbl.imag() -1) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() / exact_mpfr.real() -1) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() / exact_mpfr.imag() -1) < threshold_clearance_mp);
	
	N = a*y*x;
	BOOST_CHECK_EQUAL(N->Degree(),2);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() / exact_dbl.real() -1) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() / exact_dbl.imag() -1) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() / exact_mpfr.real() -1) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() / exact_mpfr.imag() -1) < threshold_clearance_mp);
	
}


BOOST_AUTO_TEST_CASE(manual_construction_x_divide_y){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Variable> y = std::make_shared<Variable>("y");
	
	x->set_current_value<dbl>(xnum_dbl);
	y->set_current_value<dbl>(ynum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	y->set_current_value<mpfr>(bertini::complex(ystr_real,ystr_imag));
	
	dbl exact_dbl = xnum_dbl/ynum_dbl;
	mpfr exact_mpfr = xnum_mpfr/ynum_mpfr;
	
	std::shared_ptr<Node> N = x/y;
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),-1);

	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK(! N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
	exact_dbl = ynum_dbl/xnum_dbl;
	exact_mpfr = ynum_mpfr/xnum_mpfr;
	
	N = y/x;

	BOOST_CHECK(! N->IsPolynomial());

	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
}


BOOST_AUTO_TEST_CASE(manual_construction_negate_x){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	
	x->set_current_value<dbl>(xnum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	
	dbl exact_dbl = -xnum_dbl;
	mpfr exact_mpfr = -xnum_mpfr;
	
	std::shared_ptr<Node> N = -x;
	BOOST_CHECK_EQUAL(N->Degree(),1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);

	BOOST_CHECK(N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}







/////////// Basic Operations Combined ///////////////////


BOOST_AUTO_TEST_CASE(manual_construction_lx_plus_y_plus_num1l_pow_num2){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Variable> y = std::make_shared<Variable>("y");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	std::shared_ptr<Float> p = std::make_shared<Float>(pstr_real, pstr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	y->set_current_value<dbl>(ynum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	y->set_current_value<mpfr>(bertini::complex(ystr_real,ystr_imag));
	
	dbl exact_dbl = pow(xnum_dbl+ynum_dbl+anum_dbl,pnum_dbl);
	mpfr exact_mpfr = pow(xnum_mpfr+ynum_mpfr+anum_mpfr,pnum_mpfr);
	
	std::shared_ptr<Node> N = pow(x+y+a,p);
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);
	BOOST_CHECK_EQUAL(N->Degree(y),-1);

	BOOST_CHECK(! N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_lx_minus_y_minus_num1l_pow_num2){
	using mpfr_float = boost::multiprecision::mpfr_float;
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Variable> y = std::make_shared<Variable>("y");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	std::shared_ptr<Float> p = std::make_shared<Float>(pstr_real, pstr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	y->set_current_value<dbl>(ynum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	y->set_current_value<mpfr>(bertini::complex(ystr_real,ystr_imag));
	
	dbl exact_dbl = pow(xnum_dbl-ynum_dbl-anum_dbl,pnum_dbl);
	mpfr exact_mpfr = pow(xnum_mpfr-ynum_mpfr-anum_mpfr,pnum_mpfr);
	
	std::shared_ptr<Node> N = pow(x-y-a,p);
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);
	BOOST_CHECK_EQUAL(N->Degree(y),-1);

	BOOST_CHECK(! N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_lx_times_y_times_num1l_pow_num2){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Variable> y = std::make_shared<Variable>("y");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	std::shared_ptr<Float> p = std::make_shared<Float>(pstr_real, pstr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	y->set_current_value<dbl>(ynum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	y->set_current_value<mpfr>(bertini::complex(ystr_real,ystr_imag));
	
	dbl exact_dbl = pow(xnum_dbl*ynum_dbl*anum_dbl,pnum_dbl);
	mpfr exact_mpfr = pow(xnum_mpfr*ynum_mpfr*anum_mpfr,pnum_mpfr);
	
	std::shared_ptr<Node> N = pow(x*y*a,p);
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);
	BOOST_CHECK_EQUAL(N->Degree(y),-1);

	BOOST_CHECK(! N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag()) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
	N = pow(x,p)*pow(y,p)*pow(a,p);
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);
	BOOST_CHECK_EQUAL(N->Degree(y),-1);

	exact_dbl = pow(xnum_dbl,pnum_dbl)*pow(ynum_dbl,pnum_dbl)*pow(anum_dbl,pnum_dbl);
	exact_mpfr = pow(xnum_mpfr,pnum_mpfr)*pow(ynum_mpfr,pnum_mpfr)*pow(anum_mpfr,pnum_mpfr);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() / exact_dbl.real() -1) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() / exact_dbl.imag() -1) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() / exact_mpfr.real() -1) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() / exact_mpfr.imag() -1) < threshold_clearance_mp);
	
}


BOOST_AUTO_TEST_CASE(manual_construction_lx_over_yl_pow_num2){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Variable> y = std::make_shared<Variable>("y");
	std::shared_ptr<Float> p = std::make_shared<Float>(pstr_real, pstr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	y->set_current_value<dbl>(ynum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	y->set_current_value<mpfr>(bertini::complex(ystr_real,ystr_imag));
	
	dbl exact_dbl = pow(xnum_dbl/ynum_dbl,pnum_dbl);
	mpfr exact_mpfr = pow(xnum_mpfr/ynum_mpfr,pnum_mpfr);
	
	std::shared_ptr<Node> N = pow(x/y,p);
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);
	BOOST_CHECK_EQUAL(N->Degree(y),-1);

	BOOST_CHECK(!N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
	N = pow(x,p)/pow(y,p);
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);
	BOOST_CHECK_EQUAL(N->Degree(y),-1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
}


BOOST_AUTO_TEST_CASE(manual_construction_lnegative_xl_pow_num2){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Float> p = std::make_shared<Float>(pstr_real, pstr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	
	dbl exact_dbl = pow(-xnum_dbl,pnum_dbl);
	mpfr exact_mpfr = pow(-xnum_mpfr,pnum_mpfr);
	
	std::shared_ptr<Node> N = -x;
	N = pow(N,p);
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);

	BOOST_CHECK(!N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_negate_x_plus_y_plus_num1){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Variable> y = std::make_shared<Variable>("y");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	y->set_current_value<dbl>(ynum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	y->set_current_value<mpfr>(bertini::complex(ystr_real,ystr_imag));
	
	dbl exact_dbl = -(xnum_dbl+ynum_dbl+anum_dbl);
	mpfr exact_mpfr = -(xnum_mpfr+ynum_mpfr+anum_mpfr);
	
	std::shared_ptr<Node> N = -(x+y+a);
	BOOST_CHECK_EQUAL(N->Degree(),1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
	N = -x-y-a;
	BOOST_CHECK_EQUAL(N->Degree(),1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_negate_x_minus_y_minus_num1){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Variable> y = std::make_shared<Variable>("y");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	y->set_current_value<dbl>(ynum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	y->set_current_value<mpfr>(bertini::complex(ystr_real,ystr_imag));
	
	dbl exact_dbl = -(xnum_dbl-ynum_dbl-anum_dbl);
	mpfr exact_mpfr = -(xnum_mpfr-ynum_mpfr-anum_mpfr);
	
	std::shared_ptr<Node> N = -(x-y-a);
	BOOST_CHECK_EQUAL(N->Degree(),1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
	N = -x+y+a;
	BOOST_CHECK_EQUAL(N->Degree(),1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
}



BOOST_AUTO_TEST_CASE(manual_construction_negate_x_times_y_times_num1){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Variable> y = std::make_shared<Variable>("y");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	y->set_current_value<dbl>(ynum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	y->set_current_value<mpfr>(bertini::complex(ystr_real,ystr_imag));
	
	dbl exact_dbl = -(xnum_dbl*ynum_dbl*anum_dbl);
	mpfr exact_mpfr = -(xnum_mpfr*ynum_mpfr*anum_mpfr);
	
	std::shared_ptr<Node> N = -(x*y*a);
	BOOST_CHECK_EQUAL(N->Degree(),2);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
	N = (-x)*y*a;
	BOOST_CHECK_EQUAL(N->Degree(),2);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
	N = x*(-y)*a;
	BOOST_CHECK_EQUAL(N->Degree(),2);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
	N = x*y*(-a);
	BOOST_CHECK_EQUAL(N->Degree(),2);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
}



BOOST_AUTO_TEST_CASE(manual_construction_negate_x_over_y){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Variable> y = std::make_shared<Variable>("y");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	y->set_current_value<dbl>(ynum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	y->set_current_value<mpfr>(bertini::complex(ystr_real,ystr_imag));
	
	dbl exact_dbl = -(xnum_dbl/ynum_dbl);
	mpfr exact_mpfr = -(xnum_mpfr/ynum_mpfr);
	
	std::shared_ptr<Node> N = -(x/y);
	BOOST_CHECK_EQUAL(N->Degree(),-1); 
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),-1);

	BOOST_CHECK(!N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
	
	N = -x/y;
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),-1);


	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
	
	N = x/(-y);
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),-1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	;
	
}


BOOST_AUTO_TEST_CASE(manual_construction_negate_x_pow_num2){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Float> p = std::make_shared<Float>(pstr_real, pstr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	
	dbl exact_dbl = -pow(xnum_dbl,pnum_dbl);
	mpfr exact_mpfr = -pow(xnum_mpfr,pnum_mpfr);
	
	std::shared_ptr<Node> N = pow(x,p);
	N = -N;
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);
	
	BOOST_CHECK(!N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}







/////////// Order of Operations ///////////////////

BOOST_AUTO_TEST_CASE(manual_construction_x_times_y_over_num){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Variable> y = std::make_shared<Variable>("y");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	y->set_current_value<dbl>(ynum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	y->set_current_value<mpfr>(bertini::complex(ystr_real,ystr_imag));
	
	dbl exact_dbl = xnum_dbl*ynum_dbl/anum_dbl;
	mpfr exact_mpfr = xnum_mpfr*ynum_mpfr/anum_mpfr;
	
	std::shared_ptr<Node> N = x*y/a;
	BOOST_CHECK_EQUAL(N->Degree(),2);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	


	N = (x*y)/a;
	BOOST_CHECK_EQUAL(N->Degree(),2);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);

	N = x/a*y;
	BOOST_CHECK_EQUAL(N->Degree(),2);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK_SMALL(N->Eval<dbl>().real()/exact_dbl.real()-1.0,  threshold_clearance_d);
	BOOST_CHECK_SMALL(N->Eval<dbl>().imag()/exact_dbl.imag()-1.0,  threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_lx_plus_num1l_times_ly_plus_num2l){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Variable> y = std::make_shared<Variable>("y");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	std::shared_ptr<Float> b = std::make_shared<Float>(bstr_real, bstr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	y->set_current_value<dbl>(ynum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	y->set_current_value<mpfr>(bertini::complex(ystr_real,ystr_imag));
	
	dbl exact_dbl = (xnum_dbl+anum_dbl)*(ynum_dbl+bnum_dbl);
	mpfr exact_mpfr = (xnum_mpfr+anum_mpfr)*(ynum_mpfr+bnum_mpfr);
	
	std::shared_ptr<Node> N = (x+a)*(y+b);
	BOOST_CHECK_EQUAL(N->Degree(),2);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
}


BOOST_AUTO_TEST_CASE(manual_construction_x_plus_num1_times_y_plus_num2){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Variable> y = std::make_shared<Variable>("y");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	std::shared_ptr<Float> b = std::make_shared<Float>(bstr_real, bstr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	y->set_current_value<dbl>(ynum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	y->set_current_value<mpfr>(bertini::complex(ystr_real,ystr_imag));
	
	dbl exact_dbl = xnum_dbl+anum_dbl*ynum_dbl+bnum_dbl;
	mpfr exact_mpfr = xnum_mpfr+anum_mpfr*ynum_mpfr+bnum_mpfr;
	
	std::shared_ptr<Node> N = x+a*y+b;
	BOOST_CHECK_EQUAL(N->Degree(),1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),1);

	BOOST_CHECK(N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_lx_plus_num1l_over_ly_plus_num2l){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Variable> y = std::make_shared<Variable>("y");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	std::shared_ptr<Float> b = std::make_shared<Float>(bstr_real, bstr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	y->set_current_value<dbl>(ynum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	y->set_current_value<mpfr>(bertini::complex(ystr_real,ystr_imag));
	
	dbl exact_dbl = (xnum_dbl+anum_dbl)/(ynum_dbl+bnum_dbl);
	mpfr exact_mpfr = (xnum_mpfr+anum_mpfr)/(ynum_mpfr+bnum_mpfr);
	
	std::shared_ptr<Node> N = (x+a)/(y+b);
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),-1);

	BOOST_CHECK(! N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_x_plus_num1_over_y_plus_num2){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Variable> y = std::make_shared<Variable>("y");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	std::shared_ptr<Float> b = std::make_shared<Float>(bstr_real, bstr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	y->set_current_value<dbl>(ynum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	y->set_current_value<mpfr>(bertini::complex(ystr_real,ystr_imag));
	
	dbl exact_dbl = xnum_dbl+anum_dbl/ynum_dbl+bnum_dbl;
	mpfr exact_mpfr = xnum_mpfr+anum_mpfr/ynum_mpfr+bnum_mpfr;
	
	std::shared_ptr<Node> N = x+a/y+b;
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);
	BOOST_CHECK_EQUAL(N->Degree(y),-1);

	BOOST_CHECK(! N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_lx_pow_num2l_plus_num1){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	std::shared_ptr<Float> p = std::make_shared<Float>(pstr_real, pstr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	
	dbl exact_dbl = pow(xnum_dbl,pnum_dbl)+anum_dbl;
	mpfr exact_mpfr = pow(xnum_mpfr,pnum_mpfr)+anum_mpfr;
	
	std::shared_ptr<Node> N = pow(x,p)+a;
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);

	BOOST_CHECK(! N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_x_plus_lnum1_pow_num2l){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	std::shared_ptr<Float> p = std::make_shared<Float>(pstr_real, pstr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	
	dbl exact_dbl = pow(anum_dbl,pnum_dbl)+xnum_dbl;
	mpfr exact_mpfr = pow(anum_mpfr,pnum_mpfr)+xnum_mpfr;
	
	std::shared_ptr<Node> N = x+pow(a,p);
	BOOST_CHECK_EQUAL(N->Degree(),1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);

	BOOST_CHECK(N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_x_times_lnum1_pow_num2l){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	std::shared_ptr<Float> p = std::make_shared<Float>(pstr_real, pstr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	
	dbl exact_dbl = pow(anum_dbl,pnum_dbl)*xnum_dbl;
	mpfr exact_mpfr = pow(anum_mpfr,pnum_mpfr)*xnum_mpfr;
	
	std::shared_ptr<Node> N = x*pow(a,p);
	BOOST_CHECK_EQUAL(N->Degree(),1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);

	BOOST_CHECK(N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_lx_pow_num2l_times_num1){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	std::shared_ptr<Float> p = std::make_shared<Float>(pstr_real, pstr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	
	dbl exact_dbl = pow(xnum_dbl,pnum_dbl)*anum_dbl;
	mpfr exact_mpfr = pow(xnum_mpfr,pnum_mpfr)*anum_mpfr;
	
	std::shared_ptr<Node> N = pow(x,p)*a;
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);

	BOOST_CHECK(! N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_lx_pow_num2l_over_num1){
	using mpfr_float = boost::multiprecision::mpfr_float;
	
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	std::shared_ptr<Float> p = std::make_shared<Float>(pstr_real, pstr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	
	dbl exact_dbl = pow(xnum_dbl,pnum_dbl)/anum_dbl;
	mpfr exact_mpfr = pow(xnum_mpfr,pnum_mpfr)/anum_mpfr;
	
	std::shared_ptr<Node> N = pow(x,p)/a;
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);

	BOOST_CHECK(! N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_pow_lsqrt_xl_num)
{
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);

	auto exact_dbl = pow(sqrt(xnum_dbl),anum_dbl);
	auto exact_mpfr = pow(sqrt(xnum_mpfr),anum_mpfr);

	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	x->set_current_value<mpfr>(xnum_mpfr);

	std::shared_ptr<Node> N = pow(sqrt(x),a);
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);

	N = pow(x,1/2.0); // N = sqrt(x)
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);
	N = pow(N,a); // N = sqrt(x)^a
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);

	BOOST_CHECK_SMALL(N->Eval<dbl>().real() / exact_dbl.real() -1, threshold_clearance_d);
	BOOST_CHECK_SMALL(N->Eval<dbl>().imag() - exact_dbl.imag(), 10*threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);

}


BOOST_AUTO_TEST_CASE(manual_construction_x_over_lnum1_pow_num2l){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	std::shared_ptr<Float> p = std::make_shared<Float>(pstr_real, pstr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	
	dbl exact_dbl = xnum_dbl/pow(anum_dbl,pnum_dbl);
	mpfr exact_mpfr = xnum_mpfr/pow(anum_mpfr,pnum_mpfr);
	
	std::shared_ptr<Node> N = x/pow(a,p);
	BOOST_CHECK_EQUAL(N->Degree(),1);
	BOOST_CHECK_EQUAL(N->Degree(x),1);

	BOOST_CHECK(N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_x_pow_lnum1_plus_num2l){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	std::shared_ptr<Float> p = std::make_shared<Float>(pstr_real, pstr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	
	dbl exact_dbl = pow(xnum_dbl,anum_dbl+pnum_dbl);
	mpfr exact_mpfr = pow(xnum_mpfr,anum_mpfr+pnum_mpfr);
	
	std::shared_ptr<Node> N = pow(x,a+p);
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);

	BOOST_CHECK(!N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_x_pow_lnum1_times_num2l){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	std::shared_ptr<Float> p = std::make_shared<Float>(pstr_real, pstr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	
	dbl exact_dbl = pow(xnum_dbl,anum_dbl*pnum_dbl);
	mpfr exact_mpfr = pow(xnum_mpfr,anum_mpfr*pnum_mpfr);
	
	std::shared_ptr<Node> N = pow(x,a*p);
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);

	BOOST_CHECK(!N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_x_pow_lnum1_over_num2l){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	std::shared_ptr<Float> p = std::make_shared<Float>(pstr_real, pstr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	
	dbl exact_dbl = pow(xnum_dbl,pnum_dbl/anum_dbl);
	mpfr exact_mpfr = pow(xnum_mpfr,pnum_mpfr/anum_mpfr);
	
	std::shared_ptr<Node> N = pow(x,p/a);
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);


	BOOST_CHECK(!N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}









///////////// Special Functions ///////////////////
BOOST_AUTO_TEST_CASE(manual_construction_sin_num){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	
	dbl exact_dbl = sin(anum_dbl);
	mpfr exact_mpfr = sin(anum_mpfr);
	
	std::shared_ptr<Node> N = sin(a);
	
	BOOST_CHECK_EQUAL(N->Degree(),0);

	BOOST_CHECK(N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_cos_num){
	using mpfr_float = boost::multiprecision::mpfr_float;
	
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	
	dbl exact_dbl = cos(anum_dbl);
	mpfr exact_mpfr = cos(anum_mpfr);
	
	std::shared_ptr<Node> N = cos(a);
	
	BOOST_CHECK_EQUAL(N->Degree(),0);

	BOOST_CHECK(N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_tan_num){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	
	dbl exact_dbl = tan(anum_dbl);
	mpfr exact_mpfr = tan(anum_mpfr);
	
	std::shared_ptr<Node> N = tan(a);
	
	BOOST_CHECK_EQUAL(N->Degree(),0);

	BOOST_CHECK(N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_exp_num){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	
	dbl exact_dbl = exp(anum_dbl);
	mpfr exact_mpfr = exp(anum_mpfr);
	
	std::shared_ptr<Node> N = exp(a);
	
	BOOST_CHECK_EQUAL(N->Degree(),0);

	BOOST_CHECK(N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_sqrt_num){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	
	dbl exact_dbl = sqrt(anum_dbl);
	mpfr exact_mpfr = sqrt(anum_mpfr);
	
	std::shared_ptr<Node> N = sqrt(a);
	
	BOOST_CHECK_EQUAL(N->Degree(),0);

	BOOST_CHECK(N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_sin_of_lx_plus_numl){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	
	dbl exact_dbl = sin(xnum_dbl+anum_dbl);
	mpfr exact_mpfr = sin(xnum_mpfr+anum_mpfr);
	
	std::shared_ptr<Node> N = sin(x+a);
	
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);

	BOOST_CHECK(!N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_cos_of_lx_times_numl){
	using mpfr_float = boost::multiprecision::mpfr_float;
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	
	dbl exact_dbl = cos(xnum_dbl*anum_dbl);
	mpfr exact_mpfr = cos(xnum_mpfr*anum_mpfr);
	
	std::shared_ptr<Node> N = cos(x*a);
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);

	BOOST_CHECK(!N->IsPolynomial());

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_tan_of_lx_over_numl){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	
	dbl exact_dbl = tan(xnum_dbl/anum_dbl);
	mpfr exact_mpfr = tan(xnum_mpfr/anum_mpfr);
	
	std::shared_ptr<Node> N = tan(x/a);
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);

	BOOST_CHECK(!N->IsPolynomial());
	
	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_exp_of_negative_num){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	
	dbl exact_dbl = exp(-anum_dbl);
	mpfr exact_mpfr = exp(-anum_mpfr);
	
	std::shared_ptr<Node> N = exp(-a);
	BOOST_CHECK_EQUAL(N->Degree(),0);

	BOOST_CHECK(N->IsPolynomial());
	
	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}


BOOST_AUTO_TEST_CASE(manual_construction_sqrt_of_lx_pow_numl){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Float> a = std::make_shared<Float>(astr_real, astr_imag);
	
	x->set_current_value<dbl>(xnum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));
	
	dbl exact_dbl = sqrt(pow(xnum_dbl,anum_dbl));
	mpfr exact_mpfr = sqrt(pow(xnum_mpfr,anum_mpfr));
	
	std::shared_ptr<Node> N = sqrt(pow(x,a));
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);

	BOOST_CHECK(!N->IsPolynomial());
	
	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
	
	N = pow(x,a);
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);
	N = pow(N,1/2.0);
	BOOST_CHECK_EQUAL(N->Degree(),-1);
	BOOST_CHECK_EQUAL(N->Degree(x),-1);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);
}






BOOST_AUTO_TEST_CASE(arcsine_evaluate)
{
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	auto N = asin(pow(x,2)+1);

	x->set_current_value<dbl>(xnum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));

	dbl exact_dbl = asin(pow(xnum_dbl,2)+1.0);
	mpfr exact_mpfr = asin(pow(xnum_mpfr,2)+bertini::complex(1.0));

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);

	BOOST_CHECK(!N->IsPolynomial());
}


BOOST_AUTO_TEST_CASE(arccosine_evaluate)
{
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	auto N = acos(pow(x,2)+1);

	x->set_current_value<dbl>(xnum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));

	dbl exact_dbl = acos(pow(xnum_dbl,2)+1.0);
	mpfr exact_mpfr = acos(pow(xnum_mpfr,2)+bertini::complex(1.0));

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);

	BOOST_CHECK(!N->IsPolynomial());
}

BOOST_AUTO_TEST_CASE(arctangent_evaluate)
{
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	auto N = atan(pow(x,2)+1);

	x->set_current_value<dbl>(xnum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));

	dbl exact_dbl = atan(pow(xnum_dbl,2)+1.0);
	mpfr exact_mpfr = atan(pow(xnum_mpfr,2)+bertini::complex(1.0));

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);

	BOOST_CHECK(!N->IsPolynomial());
}


BOOST_AUTO_TEST_CASE(log_evaluate)
{
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	auto N = log(pow(x,2)+1);

	x->set_current_value<dbl>(xnum_dbl);
	x->set_current_value<mpfr>(bertini::complex(xstr_real,xstr_imag));

	dbl exact_dbl = log(pow(xnum_dbl,2)+1.0);
	mpfr exact_mpfr = log(pow(xnum_mpfr,2)+bertini::complex(1.0));

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<dbl>().imag() - exact_dbl.imag() ) < threshold_clearance_d);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(fabs(N->Eval<mpfr>().imag() - exact_mpfr.imag() ) < threshold_clearance_mp);

	BOOST_CHECK(!N->IsPolynomial());
}


///////////// Special Numbers ///////////////////
BOOST_AUTO_TEST_CASE(manual_construction_pi){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	dbl exact_dbl(4*atan(1.0),0);
	mpfr exact_mpfr(mpfr_float("4.0")*atan(mpfr_float("1.0")));
	
	auto N = bertini::node::Pi();
	BOOST_CHECK_EQUAL(N->Degree(),0);

	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK_EQUAL(N->Eval<dbl>().imag(),0.0);

	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK_EQUAL(N->Eval<mpfr>().imag(),0.0);

	BOOST_CHECK(N->IsPolynomial());
}


BOOST_AUTO_TEST_CASE(manual_construction_e){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	dbl exact_dbl(exp(1.0),0);
	mpfr exact_mpfr(exp(mpfr_float("1.0")));
	
	auto N = bertini::node::E();
	BOOST_CHECK_EQUAL(N->Degree(),0);
	BOOST_CHECK(fabs(N->Eval<dbl>().real() - exact_dbl.real() ) < threshold_clearance_d);
	BOOST_CHECK(N->Eval<dbl>().imag() == 0.0);
	BOOST_CHECK(fabs(N->Eval<mpfr>().real() - exact_mpfr.real() ) < threshold_clearance_mp);
	BOOST_CHECK(N->Eval<mpfr>().imag() - exact_mpfr.imag() == 0.0);
}


BOOST_AUTO_TEST_CASE(manual_construction_i){
	using mpfr_float = boost::multiprecision::mpfr_float;
	boost::multiprecision::mpfr_float::default_precision(FUNCTION_TREE_TEST_MPFR_DEFAULT_DIGITS);
	
	dbl exact_dbl(0.0,1.0);
	mpfr exact_mpfr = mpfr("0.0","1.0");
	
	auto N = bertini::node::I();
	BOOST_CHECK_EQUAL(N->Degree(),0);

	BOOST_CHECK(N->IsPolynomial());

	BOOST_CHECK(N->Eval<dbl>().real() == 0.0);
	BOOST_CHECK(N->Eval<dbl>().imag() == 1.0);
	BOOST_CHECK(N->Eval<mpfr>().real() == 0.0);
	BOOST_CHECK(N->Eval<mpfr>().imag() == 1.0);
}





BOOST_AUTO_TEST_CASE(function_tree_combine_product_of_two_integer_powers)
{
	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
	std::shared_ptr<Node> N,M,P;

	N = pow(x,5);
	M = pow(x,2);

	P = N*M;
	BOOST_CHECK_EQUAL(P->Degree(), 7);

	BOOST_CHECK(std::dynamic_pointer_cast<bertini::node::IntegerPowerOperator>(P));

	P = N/M;
	BOOST_CHECK(!std::dynamic_pointer_cast<bertini::node::IntegerPowerOperator>(P));
	BOOST_CHECK_EQUAL(P->Degree(), -1);

	N = pow(x,3);
	M = pow(x,-1);

	P = N/M;
	BOOST_CHECK(!std::dynamic_pointer_cast<bertini::node::IntegerPowerOperator>(P));
	BOOST_CHECK_EQUAL(P->Degree(), -1);
}



BOOST_AUTO_TEST_SUITE_END()




// BOOST_AUTO_TEST_SUITE(function_tree_default_constructors)

// BOOST_AUTO_TEST_CASE(function)
// {

// 	std::shared_ptr<Variable> x = std::make_shared<Variable>("x");
//     std::shared_ptr<Node> f = std::make_shared<Function>();
//     f += x;
//     BOOST_CHECK_EQUAL(f->Eval<dbl>(),dbl(0));

// }
// BOOST_AUTO_TEST_SUITE_END()




