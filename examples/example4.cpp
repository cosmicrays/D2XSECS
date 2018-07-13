#include "d2xsec.h"
#include "lis.h"
#include "pid.h"

/**
 * @example example4.cpp
 *
 * In this example the antiproton production cross sections
 * are computed on text files
 *
 * @remark Options available for xsec model : Winkler2017, Feng2016, DiMauro2015, TanNg83
 *
 * @see https://arxiv.org/abs/1711.09616
 */

/**
 * @brief Compute the antiproton production cross-sections.
 *
 * This function computes the ap production cross-sections
 * from the interaction of primary H and He on ISM and write them in a text file,
 * where columns represent: \n
 * 1) the primary kinetic energy per nucleon in GeV/n \n
 * 2) \f$\sigma(p + H_{ISM} \rightarrow \bar{p})\f$ in mbarn \n
 * 3) \f$\sigma(He + H_{ISM} \rightarrow \bar{p})\f$ in mbarn \n
 * 4) \f$\sigma(H + He_{ISM} \rightarrow \bar{p})\f$ in mbarn
 *
 * @param model production model identifier, e.g. Winkler2017
 * @param filename output file name
 */
void get_secondary_production(std::string model, std::string filename) {
	DRAGON2::D2XSEC xsec;
	xsec.set_secondary_antiprotons(model);
	auto x_ap = xsec.create_secondary_antiprotons();
	DRAGON2::TARGET H_ISM(1), He_ISM(2);
	std::ofstream outfile(filename);
	outfile << std::scientific;
	double T_proj = 100 * GeV;
	for (double T_ap = 0.1 * GeV; T_ap <= T_proj; T_ap *= 1.1) {
		double sigma_pp = x_ap->get(DRAGON2::PID(1, 1), H_ISM, T_proj, T_ap);
		double sigma_phe = x_ap->get(DRAGON2::PID(1, 1), He_ISM, T_proj, T_ap);
		double sigma_hep = x_ap->get(DRAGON2::PID(2, 4), H_ISM, T_proj, T_ap);
		outfile << T_ap / GeV << "\t";
		outfile << sigma_pp / mbarn << "\t" << sigma_phe / mbarn << "\t" << sigma_hep / mbarn << "\n";
	}
	outfile.close();
}

/**
 * @brief Compute the non annihilating cross-sections
 *
 * This function computes the non annihilating inelastic cross-section of antiprotons
 * on H in the ISM and write it in a text file, where columns represent: \n
 * 1) the antiproton kinetic energy in GeV/n \n
 * 2) \f$\sigma_{in} - \sigma_{ann}\f$ in mbarn
 *
 * @param model production model identifier, e.g. Winkler2017
 * @param filename output file name
 */
void get_tertiary_production(std::string model, std::string filename) {
	DRAGON2::D2XSEC xsec;
	xsec.set_secondary_antiprotons(model);
	auto x_ap = xsec.create_secondary_antiprotons();
	DRAGON2::TARGET H_ISM(1), He_ISM(2);
	std::ofstream outfile(filename);
	outfile << std::scientific;
	double T_proj = 100 * GeV;
	for (double T_ap = 0.1 * GeV; T_ap < 1e4 * GeV; T_ap *= 1.1) {
		double sigma_H = x_ap->get_non_annihilating_inelastic(H_ISM, T_proj);
		outfile << T_ap / GeV << " " << sigma_H / mbarn << "\n";
	}
	outfile.close();
}

/**
 * @brief Main function for example4
 */
int main() {
	get_secondary_production("Winkler2017", "output/Winkler2017.txt");
	get_tertiary_production("Winkler2017", "output/Winkler2017_tertiary.txt");

	return 0;
}