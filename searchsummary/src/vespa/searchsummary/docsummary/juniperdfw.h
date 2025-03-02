// Copyright Yahoo. Licensed under the terms of the Apache 2.0 license. See LICENSE in the project root.

#pragma once

#include "general_result.h"
#include "resultconfig.h"
#include "docsum_field_writer.h"
#include "juniper_input.h"
#include <vespa/searchlib/util/rawbuf.h>
#include <vespa/vespalib/data/slime/inserter.h>
#include <vespa/juniper/rpinterface.h>

namespace search::docsummary {

class JuniperDFW : public DocsumFieldWriter
{
public:
    virtual bool Init(
            const char *fieldName,
            const char *langFieldName,
            const ResultConfig & config,
            const char *inputField);
protected:
    JuniperDFW(juniper::Juniper * juniper);
    ~JuniperDFW() override;

    uint32_t                         _inputFieldEnumValue;
    vespalib::string                 _input_field_name;
    std::unique_ptr<juniper::Config> _juniperConfig;
    uint32_t                         _langFieldEnumValue;
    juniper::Juniper                *_juniper;
private:
    bool IsGenerated() const override { return false; }
    JuniperDFW(const JuniperDFW &);
    JuniperDFW & operator=(const JuniperDFW &);
};


class JuniperTeaserDFW : public JuniperDFW
{
public:
    bool Init(const char *fieldName, const char *langFieldName,
              const ResultConfig & config, const char *inputField) override;
protected:
    JuniperTeaserDFW(juniper::Juniper * juniper) : JuniperDFW(juniper) { }
};


class DynamicTeaserDFW : public JuniperTeaserDFW
{
    JuniperInput getJuniperInput(GeneralResult *gres);
    vespalib::string makeDynamicTeaser(uint32_t docid,
                                       vespalib::stringref input,
                                       GetDocsumsState *state);
public:
    DynamicTeaserDFW(juniper::Juniper * juniper) : JuniperTeaserDFW(juniper) { }

    void insertField(uint32_t docid, GeneralResult *gres, GetDocsumsState *state,
                     ResType type, vespalib::slime::Inserter &target) override;
};

}

