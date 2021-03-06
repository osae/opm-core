/*
  Copyright 2012 SINTEF ICT, Applied Mathematics.

  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OPM_MISCUTILITIES_HEADER_INCLUDED
#define OPM_MISCUTILITIES_HEADER_INCLUDED

#include <vector>
#include <iosfwd>

struct Wells;
struct UnstructuredGrid;

namespace Opm
{

    class IncompPropertiesInterface;
    class BlackoilPropertiesInterface;
    class RockCompressibility;

    /// @brief Computes pore volume of all cells in a grid.
    /// @param[in]  grid      a grid
    /// @param[in]  porosity  array of grid.number_of_cells porosity values
    /// @param[out] porevol   the pore volume by cell.
    void computePorevolume(const UnstructuredGrid& grid,
                           const double* porosity,
			   std::vector<double>& porevol);

    /// @brief Computes pore volume of all cells in a grid.
    /// @param[in]  number_of_cells The number of cells of the grid.
    /// @param[in]  begin_cell_volume Iterator to the volume of the first cell.
    /// @param[in]  porosity  array of grid.number_of_cells porosity values
    /// @param[out] porevol   the pore volume by cell.
    template<class T>
    void computePorevolume(int number_of_cells,
                           T begin_cell_volume,
                           const double* porosity,
                           std::vector<double>& porevol);

    /// @brief Computes pore volume of all cells in a grid, with rock compressibility effects.
    /// @param[in]  grid      a grid
    /// @param[in]  porosity  array of grid.number_of_cells porosity values (at reference pressure)
    /// @param[in]  rock_comp rock compressibility properties
    /// @param[in]  pressure  pressure by cell
    /// @param[out] porevol   the pore volume by cell.
    void computePorevolume(const UnstructuredGrid& grid,
                           const double* porosity,
                           const RockCompressibility& rock_comp,
                           const std::vector<double>& pressure,
                           std::vector<double>& porevol);

    /// @brief Computes pore volume of all cells in a grid, with rock compressibility effects.
    /// @param[in]  number_of_cells The number of cells of the grid.
    /// @param[in]  Pointer to/ Iterator at the first cell volume.
    /// @param[in]  porosity  array of grid.number_of_cells porosity values
    /// @param[in]  rock_comp rock compressibility properties
    /// @param[in]  pressure  pressure by cell
    /// @param[out] porevol   the pore volume by cell.
    template<class T>
    void computePorevolume(int number_of_cells,
                           T begin_cell_volume,
                           const double* porosity,
                           const RockCompressibility& rock_comp,
                           const std::vector<double>& pressure,
                           std::vector<double>& porevol);

    /// @brief Computes porosity of all cells in a grid, with rock compressibility effects.
    /// @param[in]  grid               a grid
    /// @param[in]  porosity_standard  array of grid.number_of_cells porosity values (at reference presure)
    /// @param[in]  rock_comp          rock compressibility properties
    /// @param[in]  pressure           pressure by cell
    /// @param[out] porosity           porosity (at reservoir condition)
    void computePorosity(const UnstructuredGrid& grid,
                         const double* porosity_standard,
                         const RockCompressibility& rock_comp,
                         const std::vector<double>& pressure,
                         std::vector<double>& porosity);

    /// @brief Computes total saturated volumes over all grid cells.
    /// @param[in]  pv        the pore volume by cell.
    /// @param[in]  s         saturation values (for all P phases)
    /// @param[out] sat_vol   must point to a valid array with P elements,
    ///                       where P = s.size()/pv.size().
    ///                       For each phase p, we compute
    ///                       sat_vol_p = sum_i s_p_i pv_i
    void computeSaturatedVol(const std::vector<double>& pv,
			     const std::vector<double>& s,
			     double* sat_vol);


    /// @brief Computes average saturations over all grid cells.
    /// @param[in]  pv        the pore volume by cell.
    /// @param[in]  s         saturation values (for all P phases)
    /// @param[out] aver_sat  must point to a valid array with P elements,
    ///                       where P = s.size()/pv.size().
    ///                       For each phase p, we compute
    ///                       aver_sat_p = (sum_i s_p_i pv_i) / (sum_i pv_i).
    void computeAverageSat(const std::vector<double>& pv,
			   const std::vector<double>& s,
			   double* aver_sat);


    /// @brief Computes injected and produced volumes of all phases.
    /// Note 1: assumes that only the first phase is injected.
    /// Note 2: assumes that transport has been done with an
    ///         implicit method, i.e. that the current state
    ///         gives the mobilities used for the preceding timestep.
    /// @param[in]  props     fluid and rock properties.
    /// @param[in]  s         saturation values (for all P phases)
    /// @param[in]  src       if < 0: total outflow, if > 0: first phase inflow.
    /// @param[in]  dt        timestep used
    /// @param[out] injected  must point to a valid array with P elements,
    ///                       where P = s.size()/src.size().
    /// @param[out] produced  must also point to a valid array with P elements.
    void computeInjectedProduced(const IncompPropertiesInterface& props,
				 const std::vector<double>& s,
				 const std::vector<double>& src,
				 const double dt,
				 double* injected,
				 double* produced);

    /// @brief Computes total mobility for a set of saturation values.
    /// @param[in]  props     rock and fluid properties
    /// @param[in]  cells     cells with which the saturation values are associated
    /// @param[in]  s         saturation values (for all phases)
    /// @param[out] totmob    total mobilities.
    void computeTotalMobility(const Opm::IncompPropertiesInterface& props,
			      const std::vector<int>& cells,
			      const std::vector<double>& s,
			      std::vector<double>& totmob);

    /// @brief Computes total mobility and omega for a set of saturation values.
    /// @param[in]  props     rock and fluid properties
    /// @param[in]  cells     cells with which the saturation values are associated
    /// @param[in]  s         saturation values (for all phases)
    /// @param[out] totmob    total mobility
    /// @param[out] omega     fractional-flow weighted fluid densities.
    void computeTotalMobilityOmega(const Opm::IncompPropertiesInterface& props,
				   const std::vector<int>& cells,
				   const std::vector<double>& s,
				   std::vector<double>& totmob,
				   std::vector<double>& omega);


    /// @brief Computes phase mobilities for a set of saturation values.
    /// @param[in]  props     rock and fluid properties
    /// @param[in]  cells     cells with which the saturation values are associated
    /// @param[in]  s         saturation values (for all phases)
    /// @param[out] pmobc     phase mobilities (for all phases).
    void computePhaseMobilities(const Opm::IncompPropertiesInterface& props,
                                const std::vector<int>&               cells,
                                const std::vector<double>&            s    ,
                                std::vector<double>&                  pmobc);


    /// Computes the fractional flow for each cell in the cells argument
    /// @param[in] props                rock and fluid properties
    /// @param[in] cells                cells with which the saturation values are associated
    /// @param[in] saturations          saturation values (for all phases)
    /// @param[out] fractional_flow     the fractional flow for each phase for each cell.
    void computeFractionalFlow(const Opm::IncompPropertiesInterface& props,
                               const std::vector<int>& cells,
                               const std::vector<double>& saturations,
                               std::vector<double>& fractional_flows);


    /// Compute two-phase transport source terms from face fluxes,
    /// and pressure equation source terms. This puts boundary flows
    /// into the source terms for the transport equation.
    /// \param[in]  grid          The grid used.
    /// \param[in]  src           Pressure eq. source terms. The sign convention is:
    ///                           (+) positive  total inflow (positive velocity divergence)
    ///                           (-) negative  total outflow
    /// \param[in]  faceflux      Signed face fluxes, typically the result from a flow solver.
    /// \param[in]  inflow_frac   Fraction of inflow (boundary and source terms) that consists of first phase.
    ///                           Example: if only water is injected, inflow_frac == 1.0.
    ///                           Note: it is not possible (with this method) to use different fractions
    ///                           for different inflow sources, be they source terms of boundary flows.
    /// \param[in]  wells         Wells data structure, or null if no wells.
    /// \param[in]  well_perfrates  Volumetric flow rates per well perforation.
    /// \param[out] transport_src The transport source terms. They are to be interpreted depending on sign:
    ///                           (+) positive  inflow of first phase (water)
    ///                           (-) negative  total outflow of both phases
    void computeTransportSource(const UnstructuredGrid& grid,
				const std::vector<double>& src,
				const std::vector<double>& faceflux,
				const double inflow_frac,
                                const Wells* wells,
                                const std::vector<double>& well_perfrates,
				std::vector<double>& transport_src);


    /// @brief Estimates a scalar cell velocity from face fluxes.
    /// @param[in]  grid            a grid
    /// @param[in]  face_flux       signed per-face fluxes
    /// @param[out] cell_velocity   the estimated velocities.
    void estimateCellVelocity(const UnstructuredGrid& grid,
			      const std::vector<double>& face_flux,
			      std::vector<double>& cell_velocity);

    /// @brief Estimates a scalar cell velocity from face fluxes.
    /// @param[in]  number_of_cells      The number of cells of the grid
    /// @param[in]  number_of_faces      The number of cells of the grid
    /// @param[in]  begin_face_centroids Iterator pointing to first face centroid.
    /// @param[in]  face_cells           Mapping from faces to connected cells.
    /// @param[in]  dimensions           The dimensions of the grid.
    /// @param[in]  begin_cell_centroids Iterator pointing to first cell centroid.
    /// @param[in]  face_flux            signed per-face fluxes
    /// @param[out] cell_velocity        the estimated velocities.
    template<class CC, class FC, class FC1, class CV>
    void estimateCellVelocity(int number_of_cells,
                              int number_of_faces,
                              FC begin_face_centroids,
                              FC1 face_cells,
                              CC begin_cell_centroids,
                              CV begin_cell_volumes,
                              int dimension,
                              const std::vector<double>& face_flux,
                              std::vector<double>& cell_velocity);

    /// Extract a vector of water saturations from a vector of
    /// interleaved water and oil saturations.
    void toWaterSat(const std::vector<double>& sboth,
		    std::vector<double>& sw);

    /// Make a vector of interleaved water and oil saturations from
    /// a vector of water saturations.
    void toBothSat(const std::vector<double>& sw,
		   std::vector<double>& sboth);

    /// Create a src vector equivalent to a wells structure.
    /// For this to be valid, the wells must be all rate-controlled and
    /// single-perforation.
    void wellsToSrc(const Wells& wells, const int num_cells, std::vector<double>& src);

    /// Computes the WDP for each well.
    /// \param[in] wells        Wells that need their wdp calculated.
    /// \param[in] grid         The associated grid to make cell lookups.
    /// \param[in] saturations  A vector of weights for each cell for each phase
    ///                         in the grid (or well, see per_grid_cell parameter). So for cell i,
    ///                         saturations[i*densities.size() + p] should give the weight
    ///                         of phase p in cell i.
    /// \param[in] densities    Density for each phase.
    /// \param[out] wdp         Will contain, for each well, the wdp of the well.
    /// \param[in] per_grid_cell Whether or not the saturations are per grid cell or per
    ///                          well cell.
    void computeWDP(const Wells& wells, const UnstructuredGrid& grid, const std::vector<double>& saturations,
                    const double* densities, const double gravity, const bool per_grid_cell,
                    std::vector<double>& wdp);

    /// Computes the WDP for each well.
    /// \param[in] wells        Wells that need their wdp calculated.
    /// \param[in] number_of_cells The number of cells in the grid.
    /// \param[in] begin_cell_centroids Pointer/Iterator to the first cell centroid. 
    /// \param[in] saturations  A vector of weights for each cell for each phase
    ///                         in the grid (or well, see per_grid_cell parameter). So for cell i,
    ///                         saturations[i*densities.size() + p] should give the weight
    ///                         of phase p in cell i.
    /// \param[in] densities    Density for each phase.
    /// \param[out] wdp         Will contain, for each well, the wdp of the well.
    /// \param[in] per_grid_cell Whether or not the saturations are per grid cell or per
    ///                          well cell.
    template<class T>
    void computeWDP(const Wells& wells, int number_of_cells, T begin_cell_centroids,
                    const std::vector<double>& saturations,
                    const double* densities, const double gravity, const bool per_grid_cell,
                    std::vector<double>& wdp);

    /// Computes (sums) the flow rate for each well.
    /// \param[in] wells                The wells for which the flow rate should be computed.
    /// \param[in] flow_rates_per_cell  Flow rates per well cells. Should ordered the same way as
    ///                                 wells.
    /// \param[out] flow_rates_per_well Will contain the summed up flow_rates for each well.
    void computeFlowRatePerWell(const Wells& wells, const std::vector<double>& flow_rates_per_cell,
                                std::vector<double>& flow_rates_per_well);

    /// Computes the phase flow rate per well
    /// \param[in] wells The wells for which the flow rate should be computed
    /// \param[in] flow_rates_per_well_cell The total flow rate for each cell (ordered the same
    ///                                 way as the wells struct
    /// \param[in] fractional_flows    the fractional flow for each cell in each well
    /// \param[out] phase_flow_per_well Will contain the phase flow per well
    void computePhaseFlowRatesPerWell(const Wells& wells,
                                      const std::vector<double>& flow_rates_per_well_cell,
                                      const std::vector<double>& fractional_flows,
                                      std::vector<double>& phase_flow_per_well);




    /// A simple flow reporting utility, encapsulating the watercut curves.
    ///
    /// Typically call push() after every timestep to build up report,
    /// then call write() to write report as a matrix with times in the
    /// first columns, water cut in the second column and cumulative
    /// production in the last column. Units used will be the same as
    /// is passed in, no conversion is done.
    class Watercut
    {
    public:
        /// Add a report point.
        /// \param[in] time      current time in the simulation
        /// \param[in] fraction  current water cut
        /// \param[in] produced  current total cumulative production
        void push(double time, double fraction, double produced);
        /// Write report to a stream.
        /// \param[in, out] os   output stream
        void write(std::ostream& os) const;
    private:
        std::vector<double> data_;
    };




    /// Well reporting utility.
    ///
    /// This class will store, for each call to push(), the following:
    ///  - the time parameter that was passed to push()
    ///  - for each well:
    ///    - bottom hole pressure in bars
    ///    - the well total rate in cubic meters per day
    ///    - the water cut (water rate / total rate)
    ///
    /// The method write() will write these data to a stream, as a
    /// matrix with time in the first column, bhp, rate and watercut
    /// of the first well in the second through fourth columns and so
    /// on.
    class WellReport
    {
    public:
        /// Add a report point.
        /// \param[in] props           fluid and rock properties
        /// \param[in] wells           well configuration
        /// \param[in] saturation      saturations by cell and phase
        /// \param[in] time            current simulation time
        /// \param[in] well_bhp        bhp values of each well
        /// \param[in] well_perfrates  total flow at each well perforation
        void push(const IncompPropertiesInterface& props,
                  const Wells& wells,
                  const std::vector<double>& saturation,
                  const double time,
                  const std::vector<double>& well_bhp,
                  const std::vector<double>& well_perfrates);
        /// Add a report point (compressible fluids).
        /// \param[in] props           fluid and rock properties
        /// \param[in] wells           well configuration
        /// \param[in] p               pressure by cell
        /// \param[in] z               surface volumes by cell and component
        /// \param[in] s               saturations by cell and phase
        /// \param[in] time            current simulation time
        /// \param[in] well_bhp        bhp values of each well
        /// \param[in] well_perfrates  total flow at each well perforation
        void push(const BlackoilPropertiesInterface& props,
                  const Wells& wells,
                  const std::vector<double>& p,
                  const std::vector<double>& z,
                  const std::vector<double>& s,
                  const double time,
                  const std::vector<double>& well_bhp,
                  const std::vector<double>& well_perfrates);
        /// Write report to a stream.
        /// \param[in, out] os   output stream
        void write(std::ostream& os) const;
    private:
        std::vector<std::vector<double> > data_;
    };

} // namespace Opm

#include "miscUtilities_impl.hpp"
#endif // OPM_MISCUTILITIES_HEADER_INCLUDED
