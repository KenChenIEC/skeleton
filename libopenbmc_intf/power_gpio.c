/**
 * Copyright © 2016 Google Inc.
 * Copyright © 2016 IBM Corporation
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

#include "power_gpio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

gboolean read_power_gpio(GDBusConnection *connection, PowerGpio *power_gpio)
{
	GDBusProxy *proxy;
	GError *error = NULL;
	GVariant *value;
	gchar *power_btn_in_name;
	gchar *power_good_in_name;
	//gchar *latch_out_name;
	gchar *bmc_ucd_cpu0_ps_hold_name;
	gchar *bmc_ucd_pmf_resout_n_name;
	gchar *bmc_ucd_gpio_name;
	gchar *bmc_pwr_btn_out_n_name;
	gchar *bmc_cpu0_ps_hold_out_name;
	gchar *bmc_cpu0_reset_n_name;
	gchar *bmc_ucd_cpu0_reset_req_name;
	gchar *bmc_ready_name;
	GVariantIter *bmc_pwr_btn_out_n_iter;
	GVariantIter *bmc_cpu0_ps_hold_out_iter;
	GVariantIter *bmc_cpu0_reset_n_iter;
	GVariantIter *bmc_ucd_cpu0_reset_req_iter;
	GVariantIter *bmc_ready_iter;
	//GVariantIter *power_up_outs_iter;
	//GVariantIter *reset_outs_iter;
	//GVariantIter *pci_reset_outs_iter;
	//gchar *power_up_out_name;
	//gchar *reset_out_name;
	//gchar *pci_reset_out_name;
	//gboolean power_up_polarity;
	gboolean bmc_pwr_btn_out_n_polarity;
	gboolean bmc_cpu0_ps_hold_out_polarity;
	gboolean bmc_cpu0_reset_n_polarity;
	gboolean bmc_ucd_cpu0_reset_req_polarity;
	gboolean bmc_ready_polarity;
	//gboolean reset_out_polarity;
	//gboolean pci_reset_out_polarity;
	//gboolean pci_reset_out_hold;
	int i;

	proxy = g_dbus_proxy_new_sync(connection,
			G_DBUS_PROXY_FLAGS_NONE,
			NULL,							/* GDBusInterfaceInfo */
			"org.openbmc.managers.System",	/* name */
			"/org/openbmc/managers/System",	/* object path */
			"org.openbmc.managers.System",	/* interface */
			NULL,							/* GCancellable */
			&error);
	if(error != NULL) {
		fprintf(stderr, "Unable to create proxy: %s\n", error->message);
		g_error_free(error);
		return FALSE;
	}

	value = g_dbus_proxy_call_sync(proxy,
			"getPowerConfiguration",
			NULL,
			G_DBUS_CALL_FLAGS_NONE,
			-1,
			NULL,
			&error);
	if(error != NULL) {
		fprintf(stderr, "Power GPIO: call to getPowerConfiguration failed: %s\n",
				error->message);
		g_error_free(error);
		return FALSE;
	}

	g_assert(value != NULL);
	memset(power_gpio, 0, sizeof(*power_gpio));
	g_variant_get(value, "(&s&s&s&s&sa(sb)a(sb)a(sb)a(sb)a(sb))", &power_btn_in_name, &power_good_in_name, &bmc_ucd_cpu0_ps_hold_name,
			&bmc_ucd_pmf_resout_n_name, &bmc_ucd_gpio_name, &bmc_pwr_btn_out_n_iter, &bmc_cpu0_ps_hold_out_iter, &bmc_cpu0_reset_n_iter,
			&bmc_ucd_cpu0_reset_req_iter, &bmc_ready_iter);

//	g_print("Power GPIO latch output %s\n", latch_out_name);
//	if(*latch_out_name != '\0') {  /* latch is optional */
//		power_gpio->latch_out.name = strdup(latch_out_name);
//	}
	g_print("power_btn_in.name %s\n", power_btn_in_name);	
	power_gpio->power_btn_in.name = g_strdup(power_btn_in_name);

	g_print("Power GPIO power good input %s\n", power_good_in_name);
	power_gpio->power_good_in.name = g_strdup(power_good_in_name);
	
	g_print("bmc_ucd_cpu0_ps_hold.name %s \n", bmc_ucd_cpu0_ps_hold_name);
	power_gpio->bmc_ucd_cpu0_ps_hold.name = g_strdup(bmc_ucd_cpu0_ps_hold_name);
	
    g_print("bmc_ucd_pmf_resout_n.name %s \n", bmc_ucd_pmf_resout_n_name);
	power_gpio->bmc_ucd_pmf_resout_n.name = g_strdup(bmc_ucd_pmf_resout_n_name);

    g_print("bmc_ucd_gpio.name %s \n", bmc_ucd_gpio_name);
	power_gpio->bmc_ucd_gpio.name = g_strdup(bmc_ucd_gpio_name);
	
	g_variant_iter_next(bmc_pwr_btn_out_n_iter, "(&sb)", &bmc_pwr_btn_out_n_name, &bmc_pwr_btn_out_n_polarity);
	power_gpio->bmc_pwr_btn_out_n.name = g_strdup(bmc_pwr_btn_out_n_name);
	power_gpio->bmc_pwr_btn_out_n_pols = bmc_pwr_btn_out_n_polarity;
    g_print("bmc_pwr_btn_out_n.name %s \n", bmc_pwr_btn_out_n_name);
	
    g_variant_iter_next(bmc_cpu0_ps_hold_out_iter, "(&sb)", &bmc_cpu0_ps_hold_out_name, &bmc_cpu0_ps_hold_out_polarity);
    power_gpio->bmc_cpu0_ps_hold_out.name = g_strdup(bmc_cpu0_ps_hold_out_name);
    power_gpio->bmc_cpu0_ps_hold_out_pols = bmc_cpu0_ps_hold_out_polarity;
    g_print("bmc_cpu0_ps_hold_out.name %s \n", bmc_cpu0_ps_hold_out_name);

    g_variant_iter_next(bmc_cpu0_reset_n_iter, "(&sb)", &bmc_cpu0_reset_n_name, &bmc_cpu0_reset_n_polarity);
    power_gpio->bmc_cpu0_reset_n.name = g_strdup(bmc_cpu0_reset_n_name);
    power_gpio->bmc_cpu0_reset_n_pols = bmc_cpu0_reset_n_polarity;
    g_print("bmc_cpu0_reset_n.name %s \n", bmc_cpu0_reset_n_name);

    g_variant_iter_next(bmc_ucd_cpu0_reset_req_iter, "(&sb)", &bmc_ucd_cpu0_reset_req_name, &bmc_ucd_cpu0_reset_req_polarity);
    power_gpio->bmc_ucd_cpu0_reset_req.name = g_strdup(bmc_ucd_cpu0_reset_req_name);
    power_gpio->bmc_ucd_cpu0_reset_req_pols = bmc_ucd_cpu0_reset_req_polarity;
    g_print("bmc_ucd_cpu0_reset_req.name %s \n", bmc_ucd_cpu0_reset_req_name);

    g_variant_iter_next(bmc_ready_iter, "(&sb)", &bmc_ready_name, &bmc_ready_polarity);
    power_gpio->bmc_ready.name = g_strdup(bmc_ready_name);
    power_gpio->bmc_ready_req_pols = bmc_ready_polarity;
    g_print("bmc_ready.name %s \n", bmc_ready_name);

//	power_gpio->num_power_up_outs = g_variant_iter_n_children(
//			power_up_outs_iter);
//	g_print("Power GPIO %d power_up outputs\n",
//			power_gpio->num_power_up_outs);
//	power_gpio->power_up_outs = g_malloc0_n(power_gpio->num_power_up_outs,
//			sizeof(GPIO));
//	power_gpio->power_up_pols = g_malloc0_n(power_gpio->num_power_up_outs,
//			sizeof(gboolean));
//	for(i = 0; g_variant_iter_next(power_up_outs_iter, "(&sb)",
//				&power_up_out_name, &power_up_polarity);
//			i++) {
//		g_print("Power GPIO power_up[%d] = %s active %s\n", i,
//				power_up_out_name, power_up_polarity ? "HIGH" : "LOW");
//		power_gpio->power_up_outs[i].name = g_strdup(power_up_out_name);
//		power_gpio->power_up_pols[i] = power_up_polarity;
//	}
//	power_gpio->num_reset_outs = g_variant_iter_n_children(reset_outs_iter);
//	g_print("Power GPIO %d reset outputs\n", power_gpio->num_reset_outs);
//	power_gpio->reset_outs = g_malloc0_n(power_gpio->num_reset_outs, sizeof(GPIO));
//	power_gpio->reset_pols = g_malloc0_n(power_gpio->num_reset_outs,
//			sizeof(gboolean));
//	for(i = 0; g_variant_iter_next(reset_outs_iter, "(&sb)", &reset_out_name,
//				&reset_out_polarity); i++) {
//		g_print("Power GPIO reset[%d] = %s active %s\n", i, reset_out_name,
//				reset_out_polarity ? "HIGH" : "LOW");
//		power_gpio->reset_outs[i].name = g_strdup(reset_out_name);
//		power_gpio->reset_pols[i] = reset_out_polarity;
//	}
//
//	power_gpio->num_pci_reset_outs = g_variant_iter_n_children(pci_reset_outs_iter);
//	g_print("Power GPIO %d pci reset outputs\n", power_gpio->num_pci_reset_outs);
//	power_gpio->pci_reset_outs = g_malloc0_n(power_gpio->num_pci_reset_outs,
//			sizeof(GPIO));
//	power_gpio->pci_reset_pols = g_malloc0_n(power_gpio->num_pci_reset_outs,
//			sizeof(gboolean));
//	power_gpio->pci_reset_holds = g_malloc0_n(power_gpio->num_pci_reset_outs,
//			sizeof(gboolean));
//	for(i = 0; g_variant_iter_next(pci_reset_outs_iter, "(&sbb)", &pci_reset_out_name,
//				&pci_reset_out_polarity, &pci_reset_out_hold); i++) {
//		g_print("Power GPIO pci reset[%d] = %s active %s, hold - %s\n", i,
//				pci_reset_out_name,
//				pci_reset_out_polarity ? "HIGH" : "LOW",
//				pci_reset_out_hold ? "Yes" : "No");
//		power_gpio->pci_reset_outs[i].name = g_strdup(pci_reset_out_name);
//		power_gpio->pci_reset_pols[i] = pci_reset_out_polarity;
//		power_gpio->pci_reset_holds[i] = pci_reset_out_hold;
//	}

//	g_variant_iter_free(power_up_outs_iter);
//	g_variant_iter_free(reset_outs_iter);
//	g_variant_iter_free(pci_reset_outs_iter);
    g_variant_iter_free(bmc_pwr_btn_out_n_iter);
    g_variant_iter_free(bmc_cpu0_ps_hold_out_iter);
    g_variant_iter_free(bmc_cpu0_reset_n_iter);
    g_variant_iter_free(bmc_ucd_cpu0_reset_req_iter);
    g_variant_iter_free(bmc_ready_iter);

	g_variant_unref(value);

	return TRUE;
}

void free_power_gpio(PowerGpio *power_gpio) {
//	int i;
//	g_free(power_gpio->latch_out.name);
//	g_free(power_gpio->power_good_in.name);
//	for(i = 0; i < power_gpio->num_power_up_outs; i++) {
//		g_free(power_gpio->power_up_outs[i].name);
//	}
//	g_free(power_gpio->power_up_outs);
//	g_free(power_gpio->power_up_pols);
//	for(i = 0; i < power_gpio->num_reset_outs; i++) {
//		g_free(power_gpio->reset_outs[i].name);
//	}
//	g_free(power_gpio->reset_outs);
//	g_free(power_gpio->reset_pols);
//	for(i = 0; i < power_gpio->num_pci_reset_outs; i++) {
//		g_free(power_gpio->pci_reset_outs[i].name);
//	}
//	g_free(power_gpio->pci_reset_outs);
//	g_free(power_gpio->pci_reset_pols);
//	g_free(power_gpio->pci_reset_holds);

	g_free(power_gpio->power_btn_in.name);
	g_free(power_gpio->power_good_in.name);
	g_free(power_gpio->bmc_ucd_cpu0_ps_hold.name);
	g_free(power_gpio->bmc_ucd_pmf_resout_n.name);
	g_free(power_gpio->bmc_ucd_gpio.name);
	g_free(power_gpio->bmc_pwr_btn_out_n.name);
	g_free(power_gpio->bmc_cpu0_ps_hold_out.name);
	g_free(power_gpio->bmc_cpu0_reset_n.name);
	g_free(power_gpio->bmc_ucd_cpu0_reset_req.name);
	g_free(power_gpio->bmc_ready.name);
}
