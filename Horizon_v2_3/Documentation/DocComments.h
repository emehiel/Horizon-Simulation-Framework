/** \mainpage Horizon Simulation Framework
*
* \section intro_sec Introduction
* The design of Complex Systems requires a disciplined systems engineering approach. The emerging 
* field of Multidisciplinary Design Optimization, (MDO), formalizes many of the systems engineering 
* approaches for design iteration, trade study analysis and optimization. The process of using MDO 
* to verify and validate system level requirements is often referred to as Utility Analysis. Key to 
* Utility Analysis is a tool capable of integrating many disparate tools that are specialized to 
* different engineering disciplines into a standard simulation framework. The Cal Poly Space 
* Systems Simulation group proposes to build such a tool with Cutting Edge Communication, an 
* industry partner. The goal of the initial development phase will be to design a modular 
* simulation architecture and to use the tool to perform top level space system design analysis. 
* The initial effort will focus on simulating the optimal tasking of three space-based earth 
* observing systems. Each satellite will be constrained by sensor geometry, power and data capacity 
* and communications availability limitations. Since the tool will be used by industry partners, 
* the initial development phase will also include the creation of a detailed use manual and 
* software description documents. The proposed simulation framework will be used by Cutting 
* Edge Communications for Aerospace Industry program design and analysis and by Cal Poly 
* students of all levels from class work to thesis work.

* Simulation capabilities are critical to the modeling and design process. With complex systems of 
* systems, design iteration and design trade-off analysis is critical. For successful design 
* iteration to occur, a rapid simulation tool that can incorporate models of different fidelity is 
* critical. Also, the design analysis tool must be flexible, allow for integration with other 
* existing tool, and allow for system modification as subsystem model designs mature. The 
* proposed CPS3 simulation software framework will satisfy all of these goals.

* While Cutting Edge will gain a valuable tool for space system analysis and new capabilities for 
* servicing contracts, Cal Poly students will gain industrial experience developing real software 
* modules, and the Aerospace Engineering curriculum will gain a valuable tool for senior design, 
* senior projects and Space Systems Masters Students and other MS degree programs. In fact, the 
* Cal Poly Aerospace Department takes great pride in our student’s ability to do spacecraft and 
* aircraft design; our entire curriculum is focused around the Senior Design Lab. The CPS3 
* software framework will enable our students to engage in meaningful, realistic Multidisciplinary 
* Design and Optimization.

* Version 1.0 completed 1.6.2007
* Version 1.1 completed 1.29.2007

* @version     V1.2

* @author Eric Mehiel
* @author Cory O'Connor
* @author Einar Pehrson
* @author Travis Lockyer
* @author Dan Treachler
* @author Seth Silva
*/

/**
* The horizon namespace holds most of the framework and main executable code.
*/
namespace horizon {
	
	/** 
	* Holds the classes containing the different constraints on the system. The user must
	* add classes to this namespace when they wish to add new constraints to the system.
	*/
	namespace constraints {}
	
	/** Holds the different methods of evaluation for schedules. */
	namespace eval {}
	
	/** Holds the geometric, universal, and evaluative methods for the simulation. */
	namespace geom {}
	
	/** Input/Output classes are held here, most notably the XML reader for input nodes. */ 
	namespace io {}
	
	/** Subsystem classes are stored in the sub namespace. */ 
	namespace sub {
		
		/** The dependencies between Subsystems are kept in this namespace. */ 
		namespace dep {}
	}		
	
	/** A temporary namespace used to hold placeholder classes for UI creation */
	namespace ui {}

	/** 
	* A utility namespace where numerical methods, container types, mathematical classes,
	* and utility math functions are held 
	*/
	namespace util {
	
		/**
		* This class contains the equations of motion objects necessary for integration using
		* either of the Runge-Kutta integration methods. Also included are the input classes
		* to the EOMS objects, necessary for non-constant inputs.
		*/
		namespace eoms {}

		/**
		* This class contains the different methods, return types, and integration parameters
		* necessary for integration in the horizon framework. Currently included are the following
		* methods: Runge-Kutta45, Runge-Kutta, Trapezoidal.
		*/
		namespace integrate {}

		/**
		* The classes taken from the Template Numerical Toolkit (a c++ rewrite of the original
		* fortran77 numerical methods LAPACK and BLAS) are held here. Also here are transfer
		* methods from the Matrix class to and from the specific container classes used in TNT.
		*/
		namespace tnt {}

		/**
		* A collection of various global functions that perform mathematical
		* operations or transformations.
		*/
		namespace umath {
			
			/**
			 * The methods for evaluating different types of splines are held here.
			 */
			namespace spline {}
		}
	}
}

/**
* The testing namespace is where the test classes for cppunit testing reside. At the moment
* only classes and functions called or directly related to the main algorithm are being
* tested, but eventually all classes should be added.
*/
namespace testing {}