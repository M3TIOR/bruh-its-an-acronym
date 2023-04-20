// SPDX-Package-Home-Page: https://github.com/dafky2000/simplectemplate
// SPDX-Copyright-Text: Copyright 2017 Daniel Kelly <myself@danielkelly.me>
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-File-Notice: Trimmed version to suit this project.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
struct template {
	union {
		struct { char** keys, values; };
		const struct* { const char* key, value; };
	};
};

// You must free the result if result is non-NULL.
static char* str_replace(char* orig, struct template) {

	char* result;  // the return string
	char* ins;     // the next insert point
	char* tmp;     // varies
	int len_rep;   // length of rep (the string to remove)
	int len_with;  // length of with (the string to replace rep with)
	int count;     // number of replacements

	// sanity checks and initialization
	if(!orig || !rep) return NULL;

	len_rep = strlen(rep);
	if(len_rep == 0) return NULL; // empty rep causes infinite loop during count

	if(!with) with = "";
	len_with = strlen(with);

	// count the number of replacements needed
	ins = orig;     // the next insert point
	for(count = 0; (tmp = strstr(ins, rep)); ++count) {
		ins = tmp + len_rep;
	}

	tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

	if(!result) return NULL;

	// first time through the loop, all the variable are set correctly from here on,
	//    tmp points to the end of the result string
	//    ins points to the next occurrence of rep in orig
	//    orig points to the remainder of orig after "end of rep"
	while(count--) {
			ins = strstr(orig, rep);
			int len_front = ins - orig;
			tmp = strncpy(tmp, orig, len_front) + len_front;
			tmp = strcpy(tmp, with) + len_with;
			orig += len_front + len_rep; // move to next "end of rep"
	}
	strcpy(tmp, orig);

	return result;
}
*/

// "Stolen" from https://stackoverflow.com/questions/779875/what-is-the-function-to-replace-string-in-c
/**
 * Replace all occurances of rep
 * orig: text to perfom search and replace on
 * rep: text to search for
 * with: text to replace with
 * returns: replaced text
 */
// You must free the result if result is non-NULL.
static char* str_replace(char* orig, const char* rep, const char* with) {
	char* result;  // the return string
	char* ins;     // the next insert point
	char* tmp;     // varies
	int len_rep;   // length of rep (the string to remove)
	int len_with;  // length of with (the string to replace rep with)
	int count;     // number of replacements

	// sanity checks and initialization
	if(!orig || !rep) return NULL;

	len_rep = strlen(rep);
	if(len_rep == 0) return NULL; // empty rep causes infinite loop during count

	if(!with) with = "";
	len_with = strlen(with);

	// count the number of replacements needed
	ins = orig;     // the next insert point
	for(count = 0; (tmp = strstr(ins, rep)); ++count) {
		ins = tmp + len_rep;
	}

	tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

	if(!result) return NULL;

	// first time through the loop, all the variable are set correctly from here on,
	//    tmp points to the end of the result string
	//    ins points to the next occurrence of rep in orig
	//    orig points to the remainder of orig after "end of rep"
	while(count--) {
			ins = strstr(orig, rep);
			int len_front = ins - orig;
			tmp = strncpy(tmp, orig, len_front) + len_front;
			tmp = strcpy(tmp, with) + len_with;
			orig += len_front + len_rep; // move to next "end of rep"
	}
	strcpy(tmp, orig);

	return result;
}

/**
 * Performs a str_replace using the correct formatting with just key/value pair
 * template: the template contents with placeholders to be replaced
 * key: the placeholder identifier
 * value: value to replace with
 * returns: Replaced template
 */
static char* set_template_var(char* template, const char* key, const char* value) {
	int keylen = strlen("{{}}") + strlen(key) + 1;
	char* fullkey = malloc(keylen);
	if (!fullkey) return NULL;
	//memset(fullkey, 0, keylen);

	strcat(fullkey, "{{");
	strcat(fullkey, key);
	strcat(fullkey, "}}");
	char* ret = str_replace(template, fullkey, value);
	free(fullkey);
	return ret;
}

char* render_template(const char* template_data, int len, const char* keys[], const char* values[]) {
	int template_length = strlen(template_data) + 1;
	char* template = calloc(template_length);
	if (!template) return NULL; 
	//memset(template, 0, template_length);

	strcpy(template, template_data);

	
	for(int x=0; x < len; x++) {
		char* processed = set_template_var(template, keys[x], values[x]);
		if (!processed) return NULL;
		free(template);
		template = processed;
	}

	return template;
}

char* render_template2(const char* template_data, int len, const char* data[]) {
	const char* keys[len];
	const char* values[len];

	int i;
	for(i = 0; i < len; i++) {
		keys[i] = (char *)data[i*2];
		values[i] = (char *)data[i*2+1];
	}

	return render_template(template_data, len, keys, values);
}
