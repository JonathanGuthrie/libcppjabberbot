/*
 * Copyright 2010 Jonathan R. Guthrie
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <sstream>

#include "stanza.hpp"
#include "iq-stanza.hpp"
#include "presence-stanza.hpp"
#include "message-stanza.hpp"

Stanza::Stanza(void) {
  m_to = NULL;
  m_from = NULL;
  m_type = NULL;
  m_id = NULL;
  m_namespace = NULL;
  m_errorMessage = NULL;
  m_error = 200;
}

Stanza::~Stanza(void) {
  delete m_to;
  delete m_from;
  delete m_type;
  delete m_id;
  delete m_namespace;
  delete m_errorMessage;
}


void Stanza::To(const std::string *to) {
  delete m_to;
  m_to = to;
}


void Stanza::To(const std::string &to) {
  To(new std::string(to));
}


void Stanza::From(const std::string *from) {
  delete m_from;
  m_from = from;
}


void Stanza::From(const std::string &from) {
  From(new std::string(from));
}


void Stanza::Type(const std::string *type) {
  delete m_type;
  m_type = type;
}


void Stanza::Type(const std::string &type) {
  Type(new std::string(type));
}


void Stanza::Id(const std::string *id) {
  delete m_id;
  m_id = id;
}


void Stanza::Id(const std::string &id) {
  Id(new std::string(id));
}


void Stanza::Namespace(const std::string *name_space) {
  delete m_namespace;
  m_namespace = name_space;
}


void Stanza::Namespace(const std::string &name_space) {
  Namespace(new std::string(name_space));
}


void Stanza::ErrorMessage(const std::string *errorMessage) {
  delete m_errorMessage;
  m_errorMessage = errorMessage;
}


void Stanza::ErrorMessage(const std::string &errorMessage) {
  ErrorMessage(new std::string(errorMessage));
}


void Stanza::Error(int error) {
  m_error = error;
}


const std::string *Stanza::To(void) const {
  return m_to;
}


const std::string *Stanza::From(void) const {
  return m_from;
}


const std::string *Stanza::Type(void) const {
  return m_type;
}


const std::string *Stanza::Id(void) const  {
  return m_id;
}


const std::string *Stanza::Namespace(void) const  {
  return m_namespace;
}


const std::string *Stanza::ErrorMessage(void) const {
  return m_errorMessage;
}


int Stanza::Error(void) const {
  return m_error;
}


const std::string *Stanza::renderStanza(const std::string *id, const std::string &tag, const std::string &body) const {
  std::ostringstream result;

  result << "<" << tag;
  if (NULL != id) {
    result << " id='" << *id << "'";
  }
  if (NULL != m_to) {
    result << " to='" << *m_to << "'";
  }
  if (NULL != m_from) {
    result << " from='" << *m_from << "'";
  }
  if (NULL != m_type) {
    result << " type='" << *m_type << "'";
  }
  if ("" != body) {
    result << ">" << body << "</" << tag << ">";
  }
  else {
    result << "/>";
  }
  return new std::string(result.str());
}

Stanza *Stanza::parse(const JabberElementNode *root) {
  Stanza *result = NULL;

  if ("iq" == root->m_name) {
    result = IqStanza::parse(root);
  }
  if ("presence" == root->m_name) {
    result = PresenceStanza::parse(root);
  }
  if ("message" == root->m_name) {
    result = MessageStanza::parse(root);
  }
  return result;
}
