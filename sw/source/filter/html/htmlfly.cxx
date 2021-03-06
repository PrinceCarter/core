/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */

#include "htmlfly.hxx"

#include <fmtanchr.hxx>
#include <fmtornt.hxx>
#include <flypos.hxx>

#include <frmfmt.hxx>
#include <ndindex.hxx>
#include <pam.hxx>
#include <osl/diagnose.h>

using namespace css;

SwHTMLPosFlyFrame::SwHTMLPosFlyFrame( const SwPosFlyFrame& rPosFly,
                                  const SdrObject *pSdrObj,
                                  AllHtmlFlags nFlags ) :
    pFrameFormat( &rPosFly.GetFormat() ),
    pSdrObject( pSdrObj ),
    pNdIdx( new SwNodeIndex( rPosFly.GetNdIndex() ) ),
    nOrdNum( rPosFly.GetOrdNum() ),
    nContentIdx( 0 ),
    nAllFlags( nFlags )
{
    const SwFormatAnchor& rAnchor = rPosFly.GetFormat().GetAnchor();
    if ((RndStdIds::FLY_AT_CHAR == rAnchor.GetAnchorId()) &&
        HtmlPosition::Inside == GetOutPos() )
    {
        // Output of auto-bound frames will be a character farther back,
        // because then the position aligns with Netscape.
        OSL_ENSURE( rAnchor.GetContentAnchor(), "No anchor position?" );
        if( rAnchor.GetContentAnchor() )
        {
            nContentIdx = rAnchor.GetContentAnchor()->nContent.GetIndex();
            sal_Int16 eHoriRel = rPosFly.GetFormat().GetHoriOrient().
                                                GetRelationOrient();
            if( text::RelOrientation::FRAME == eHoriRel || text::RelOrientation::PRINT_AREA == eHoriRel )
            {
                const SwContentNode *pCNd = pNdIdx->GetNode().GetContentNode();
                OSL_ENSURE( pCNd, "No Content-Node at PaM position" );
                if( pCNd && nContentIdx < pCNd->Len() )
                    nContentIdx++;
            }
        }
    }
}

bool SwHTMLPosFlyFrame::operator<( const SwHTMLPosFlyFrame& rFrame ) const
{
    if( pNdIdx->GetIndex() == rFrame.pNdIdx->GetIndex() )
    {
        if( nContentIdx == rFrame.nContentIdx )
        {
            if( GetOutPos() == rFrame.GetOutPos() )
                return nOrdNum < rFrame.nOrdNum;
            else
                return GetOutPos() < rFrame.GetOutPos();
        }
        else
            return nContentIdx < rFrame.nContentIdx;
    }
    else
        return pNdIdx->GetIndex() < rFrame.pNdIdx->GetIndex();
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
