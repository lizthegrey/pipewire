/* PipeWire
 * Copyright (C) 2018 Wim Taymans <wim.taymans@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <string.h>

#include <spa/debug/mem.h>

#include <pipewire/log.h>
#include <pipewire/properties.h>

#include <pulse/proplist.h>

#include "internal.h"
#include "strbuf.h"

struct pa_proplist {
	struct pw_properties *props;
};

int pa_proplist_update_dict(pa_proplist *p, struct spa_dict *dict)
{
	const struct spa_dict_item *item;
	spa_dict_for_each(item, dict)
		pa_proplist_sets(p, item->key, item->value);
	return 0;
}

pa_proplist* pa_proplist_new_dict(struct spa_dict *dict)
{
	pa_proplist *p;

	p = pa_proplist_new();
	if (p == NULL)
		return NULL;

	pa_proplist_update_dict(p, dict);

	return p;
}
pa_proplist* pa_proplist_new_props(struct pw_properties *props)
{
	return pa_proplist_new_dict(&props->dict);
}

SPA_EXPORT
pa_proplist* pa_proplist_new(void)
{
	pa_proplist *p;

	p = calloc(1, sizeof(struct pa_proplist));
	if (p == NULL)
		return NULL;

	p->props = pw_properties_new(NULL, NULL);

	return p;
}

SPA_EXPORT
void pa_proplist_free(pa_proplist* p)
{
	pw_properties_free(p->props);
	free(p);
}

SPA_EXPORT
int pa_proplist_key_valid(const char *key)
{
	const char *p;
	for (p = key; *p; p++)
	        if ((unsigned char) *p >= 128)
			return 0;

	if (strlen(key) < 1)
		return 0;

	return 1;
}

SPA_EXPORT
int pa_proplist_sets(pa_proplist *p, const char *key, const char *value)
{
	pa_assert(p);
	pa_assert(key);
	pa_assert(value);

	if (!pa_proplist_key_valid(key))
		return -1;

	pw_properties_set(p->props, key, value);
	return 0;
}

SPA_EXPORT
int pa_proplist_setp(pa_proplist *p, const char *pair)
{
	const char *t;
	char *c;
	int idx;

	pa_assert(p);
	pa_assert(pair);

	if (!(t = strchr(pair, '=')))
		return -1;

	idx = pair - t;
	c = strdup(pair);
	c[idx] = 0;
	pa_proplist_sets(p, c, &c[idx]+1);
	free(c);

	return 0;
}

SPA_EXPORT
int pa_proplist_setf(pa_proplist *p, const char *key, const char *format, ...)
{
	va_list varargs;

	va_start(varargs, format);
	pw_properties_setva(p->props, key, format, varargs);
	va_end(varargs);

	return 0;
}

SPA_EXPORT
int pa_proplist_set(pa_proplist *p, const char *key, const void *data, size_t nbytes)
{
	pa_assert(p);
	pa_assert(key);
	pa_assert(data || nbytes == 0);

	if (!pa_proplist_key_valid(key))
		return -1;

	pw_properties_set(p->props, key, data);
	return 0;
}

SPA_EXPORT
const char *pa_proplist_gets(PA_CONST pa_proplist *p, const char *key)
{
	return pw_properties_get(p->props, key);
}

SPA_EXPORT
int pa_proplist_get(PA_CONST pa_proplist *p, const char *key, const void **data, size_t *nbytes)
{
	const char *val;

	spa_assert(p);
	spa_assert(key);

	val = pw_properties_get(p->props, key);

	*data = val;
	*nbytes = val ? strlen(val) : 0;
	return 0;
}

SPA_EXPORT
void pa_proplist_update(pa_proplist *p, pa_update_mode_t mode, const pa_proplist *other)
{
	uint32_t i;

	spa_assert(p);
	spa_assert(mode == PA_UPDATE_SET || mode == PA_UPDATE_MERGE || mode == PA_UPDATE_REPLACE);
	spa_assert(other);

	if (mode == PA_UPDATE_REPLACE) {
		pa_proplist_update_dict(p, &other->props->dict);
		return;
	}

	if (mode == PA_UPDATE_SET)
		pa_proplist_clear(p);

	for (i = 0; i < other->props->dict.n_items; i++) {
		const struct spa_dict_item *oi;

		oi = &other->props->dict.items[i];
		if (pw_properties_get(p->props, oi->key) == NULL)
			pw_properties_set(p->props, oi->key, oi->value);
	}
}

SPA_EXPORT
int pa_proplist_unset(pa_proplist *p, const char *key)
{
	spa_assert(p);
	spa_assert(key);

	if (!pa_proplist_key_valid(key))
		return -1;

	return pw_properties_set(p->props, key, NULL);
}

SPA_EXPORT
int pa_proplist_unset_many(pa_proplist *p, const char * const keys[])
{
	const char * const * k;
	int n = 0;

	spa_assert(p);
	spa_assert(keys);

	for (k = keys; *k; k++)
		if (!pa_proplist_key_valid(*k))
			return -1;

	for (k = keys; *k; k++)
		if (pa_proplist_unset(p, *k) >= 0)
			n++;
	return n;
}

SPA_EXPORT
const char *pa_proplist_iterate(PA_CONST pa_proplist *p, void **state)
{
	spa_assert(p);
	spa_assert(state);
	return pw_properties_iterate(p->props, state);
}

SPA_EXPORT
char *pa_proplist_to_string(PA_CONST pa_proplist *p)
{
	spa_assert(p);
	return pa_proplist_to_string_sep(p, ",");
}

SPA_EXPORT
char *pa_proplist_to_string_sep(PA_CONST pa_proplist *p, const char *sep)
{
	const char *key;
	void *state = NULL;
	pa_strbuf *buf;

	spa_assert(p);
	spa_assert(sep);

	buf = pa_strbuf_new();

	while ((key = pa_proplist_iterate(p, &state))) {
		const char *v;
		const char *t;

		if (!pa_strbuf_isempty(buf))
			pa_strbuf_puts(buf, sep);

		if ((v = pa_proplist_gets(p, key)) == NULL)
			continue;

		pa_strbuf_printf(buf, "%s = \"", key);

		for (t = v;;) {
			size_t h;

			h = strcspn(t, "\"");

			if (h > 0)
				pa_strbuf_putsn(buf, t, h);

			t += h;

			if (*t == 0)
				break;

			pa_assert(*t == '"');
			pa_strbuf_puts(buf, "\\\"");

			t++;
		}
		pa_strbuf_puts(buf, "\"");
	}
	return pa_strbuf_to_string_free(buf);
}

SPA_EXPORT
pa_proplist *pa_proplist_from_string(const char *str)
{
	spa_assert(str);
	pw_log_warn("Not Implemented");
	return NULL;
}

SPA_EXPORT
int pa_proplist_contains(PA_CONST pa_proplist *p, const char *key)
{
	spa_assert(p);
	spa_assert(key);

	if (!pa_proplist_key_valid(key))
		return -1;

	if (pw_properties_get(p->props, key) == NULL)
		return 0;

	return 1;
}

SPA_EXPORT
void pa_proplist_clear(pa_proplist *p)
{
	spa_assert(p);
	pw_properties_clear(p->props);
}

SPA_EXPORT
pa_proplist* pa_proplist_copy(const pa_proplist *p)
{
	pa_proplist *c;

	spa_assert(p);

	c = calloc(1, sizeof(struct pa_proplist));
	if (c == NULL)
		return NULL;

	c->props = pw_properties_copy(p->props);
	return c;
}

SPA_EXPORT
unsigned pa_proplist_size(PA_CONST pa_proplist *p)
{
	spa_assert(p);
	return p->props->dict.n_items;
}

SPA_EXPORT
int pa_proplist_isempty(PA_CONST pa_proplist *p)
{
	spa_assert(p);
	return p->props->dict.n_items == 0 ? 1 : 0;
}

SPA_EXPORT
int pa_proplist_equal(PA_CONST pa_proplist *a, PA_CONST pa_proplist *b)
{
	uint32_t i;

	spa_assert(a);
	spa_assert(b);

	if (a == b)
		return 1;

	if (pa_proplist_size(a) != pa_proplist_size(b))
		return 0;

	for (i = 0; i < a->props->dict.n_items; i++) {
		const struct spa_dict_item *ai, *bi;

		ai = &a->props->dict.items[i];
		bi = spa_dict_lookup_item(&b->props->dict, ai->key);

		if (bi == NULL || bi->value == NULL || ai->value == NULL)
			return 0;
		if (strcmp(ai->value, bi->value) != 0)
			return 0;
	}
	return 1;
}


int pw_properties_update_proplist(struct pw_properties *props,
		     const pa_proplist *p)
{
	void *state = NULL;
	const char *key, *val;
	int changed = 0;

	while (true) {
		key = pa_proplist_iterate(p, &state);
		if (key == NULL)
			break;

		val = pa_proplist_gets(p, key);
		if (val == NULL)
			continue;

		changed += pw_properties_set(props, key, val);
	}
	return changed;
}
