/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <test/bootstrapfixture.hxx>

#include <com/sun/star/style/NumberingType.hpp>
#include <com/sun/star/text/DefaultNumberingProvider.hpp>
#include <com/sun/star/text/XNumberingFormatter.hpp>

#include <comphelper/propertyvalue.hxx>

using namespace ::com::sun::star;

/// i18npool defaultnumberingprovider tests.
class I18npoolDefaultnumberingproviderTest : public test::BootstrapFixture
{
};

CPPUNIT_TEST_FIXTURE(I18npoolDefaultnumberingproviderTest, testArabicZero)
{
    // 1 -> "01"
    uno::Reference<text::XNumberingFormatter> xFormatter(
        text::DefaultNumberingProvider::create(mxComponentContext), uno::UNO_QUERY);
    uno::Sequence<beans::PropertyValue> aProperties = {
        comphelper::makePropertyValue("NumberingType",
                                      static_cast<sal_uInt16>(style::NumberingType::ARABIC_ZERO)),
        comphelper::makePropertyValue("Value", static_cast<sal_Int32>(1)),
    };
    lang::Locale aLocale;
    OUString aActual = xFormatter->makeNumberingString(aProperties, aLocale);
    // Without the accompanying fix in place, this test would have failed with a
    // lang.IllegalArgumentException, support for ARABIC_ZERO was missing.
    CPPUNIT_ASSERT_EQUAL(OUString("01"), aActual);

    // 10 -> "10"
    aProperties = {
        comphelper::makePropertyValue("NumberingType",
                                      static_cast<sal_uInt16>(style::NumberingType::ARABIC_ZERO)),
        comphelper::makePropertyValue("Value", static_cast<sal_Int32>(10)),
    };
    aActual = xFormatter->makeNumberingString(aProperties, aLocale);
    CPPUNIT_ASSERT_EQUAL(OUString("10"), aActual);
}

CPPUNIT_PLUGIN_IMPLEMENT();

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
