/*
 * This file has been generated by the onto2vocabularyclass tool
 * copyright (C) 2007-2010 Sebastian Trueg <trueg@kde.org>
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
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef _SOPRANO_NBIB_H_
#define _SOPRANO_NBIB_H_

#include <QtCore/QUrl>

namespace Nepomuk {
    namespace Vocabulary {
        namespace NBIB {
            /**
             * http://www.example.com/nbib#
             */
            QUrl nbibNamespace();

            /**
             * http://www.example.com/nbib#
             */
            QUrl nrlOntologyGraph();

            /**
             * http://www.example.com/nbib#Article 
             * 
             * An article from a journal or magazine. 
             */
            QUrl Article();

            /**
             * http://www.example.com/nbib#BachelorThesis 
             * 
             * A Bachelor thesis. 
             */
            QUrl BachelorThesis();

            /**
             * http://www.example.com/nbib#Bill 
             * 
             * Draft legislation presented for discussion to a legal body. 
             */
            QUrl Bill();

            /**
             * http://www.example.com/nbib#Blog 
             * 
             * Reference the blog in which several BlogPost can be found 
             */
            QUrl Blog();

            /**
             * http://www.example.com/nbib#BlogPost 
             * 
             * Reference the post in a web log (blog) 
             */
            QUrl BlogPost();

            /**
             * http://www.example.com/nbib#Book 
             * 
             * A book with an explicit publisher. 
             */
            QUrl Book();

            /**
             * http://www.example.com/nbib#BookSeries 
             * 
             * The series of a book. 
             */
            QUrl BookSeries();

            /**
             * http://www.example.com/nbib#Booklet 
             * 
             * A work that is printed and bound, but without a named publisher 
             * or sponsoring institution. 
             */
            QUrl Booklet();

            /**
             * http://www.example.com/nbib#Brief 
             * 
             * A written argument submitted to a court. 
             */
            QUrl Brief();

            /**
             * http://www.example.com/nbib#Chapter 
             * 
             * A chapter of a publication. 
             */
            QUrl Chapter();

            /**
             * http://www.example.com/nbib#CodeOfLaw 
             * 
             * A collection of statutes. 
             */
            QUrl CodeOfLaw();

            /**
             * http://www.example.com/nbib#Collection 
             * 
             * A collection of different publications. 
             */
            QUrl Collection();

            /**
             * http://www.example.com/nbib#CourtReporter 
             * 
             * A collection of legal cases. 
             */
            QUrl CourtReporter();

            /**
             * http://www.example.com/nbib#Decision 
             * 
             * A document containing an authoritative determination (as 
             * a decree or judgment) made after consideration of facts or law. 
             */
            QUrl Decision();

            /**
             * http://www.example.com/nbib#Dictionary 
             * 
             * A dictionary 
             */
            QUrl Dictionary();

            /**
             * http://www.example.com/nbib#DocumentPart 
             * 
             * a part of an document, mostly Chapters but alsow quotes etc. 
             * These don't have to be available as real file extracts but can 
             * also be linked to a real file, if available 
             */
            QUrl DocumentPart();

            /**
             * http://www.example.com/nbib#DocumentStatus 
             * 
             * The status of the publication of a document. 
             */
            QUrl DocumentStatus();

            /**
             * http://www.example.com/nbib#Electronic 
             * 
             * An electronic publication (for example website or presentations) 
             */
            QUrl Electronic();

            /**
             * http://www.example.com/nbib#Encyclopedia 
             * 
             * A encyclopedia 
             */
            QUrl Encyclopedia();

            /**
             * http://www.example.com/nbib#Forum 
             * 
             * Reference the forum in which several ForumPost can be found 
             */
            QUrl Forum();

            /**
             * http://www.example.com/nbib#ForumPost 
             * 
             * Reference the post in a forum or other electronic list 
             */
            QUrl ForumPost();

            /**
             * http://www.example.com/nbib#Journal 
             * 
             * A Journal which publish single journal issues. 
             */
            QUrl Journal();

            /**
             * http://www.example.com/nbib#JournalIssue 
             * 
             * A journal issue for serious, scholarly publications 
             */
            QUrl JournalIssue();

            /**
             * http://www.example.com/nbib#LegalCaseDocument 
             * 
             * A document accompanying a legal case. 
             */
            QUrl LegalCaseDocument();

            /**
             * http://www.example.com/nbib#LegalDocument 
             * 
             * A legal document; for example, a court decision, a brief, and 
             * so forth. 
             */
            QUrl LegalDocument();

            /**
             * http://www.example.com/nbib#Legislation 
             * 
             * A legal document proposing or enacting a law or a group of laws. 
             */
            QUrl Legislation();

            /**
             * http://www.example.com/nbib#Magazin 
             * 
             * A Magazin which publish several MagazinIssue containing several 
             * articles 
             */
            QUrl Magazin();

            /**
             * http://www.example.com/nbib#MagazinIssue 
             * 
             * A periodical of magazine Articles. A magazine is a publication 
             * that is issued periodically, usually bound in a paper cover, 
             * and typically contains essays, stories, poems, etc., by many 
             * writers, and often photographs and drawings, frequently specializing 
             * in a particular subject or area, as hobbies, news, or sports. 
             */
            QUrl MagazinIssue();

            /**
             * http://www.example.com/nbib#Manual 
             * 
             * Technical documentation. 
             */
            QUrl Manual();

            /**
             * http://www.example.com/nbib#Map 
             * 
             * A map with geographical data 
             */
            QUrl Map();

            /**
             * http://www.example.com/nbib#MastersThesis 
             * 
             * A Master's thesis. 
             */
            QUrl MastersThesis();

            /**
             * http://www.example.com/nbib#Newspaper 
             * 
             * A Newspaper which publish several NewspaperIssue containing 
             * several articles 
             */
            QUrl Newspaper();

            /**
             * http://www.example.com/nbib#NewspaperIssue 
             * 
             * A single issue of a published Newspaper 
             */
            QUrl NewspaperIssue();

            /**
             * http://www.example.com/nbib#Patent 
             * 
             * A patent. 
             */
            QUrl Patent();

            /**
             * http://www.example.com/nbib#PhdThesis 
             * 
             * A PhD thesis. 
             */
            QUrl PhdThesis();

            /**
             * http://www.example.com/nbib#Presentation 
             * 
             * Slides for a presentation 
             */
            QUrl Presentation();

            /**
             * http://www.example.com/nbib#Proceedings 
             * 
             * The proceedings of a conference. 
             */
            QUrl Proceedings();

            /**
             * http://www.example.com/nbib#Publication 
             * 
             * A bibliographic reference resource type (article, book, paper 
             * and such) 
             */
            QUrl Publication();

            /**
             * http://www.example.com/nbib#Quote 
             * 
             * A quote in a document 
             */
            QUrl Quote();

            /**
             * http://www.example.com/nbib#Reference 
             * 
             * A bibliographic reference 
             */
            QUrl Reference();

            /**
             * http://www.example.com/nbib#Report 
             * 
             * A document describing an account or statement describing in 
             * detail an event, situation, or the like, usually as the result 
             * of observation, inquiry, etc.. 
             */
            QUrl Report();

            /**
             * http://www.example.com/nbib#Script 
             * 
             * The script for something (a lecture for ecxample) 
             */
            QUrl Script();

            /**
             * http://www.example.com/nbib#Series 
             * 
             * Superclass to represent a series of some published data. (Book 
             * Series, Journal with several issues etc) 
             */
            QUrl Series();

            /**
             * http://www.example.com/nbib#Standard 
             * 
             * A document describing a standard 
             */
            QUrl Standard();

            /**
             * http://www.example.com/nbib#Statute 
             * 
             * A bill enacted into law. 
             */
            QUrl Statute();

            /**
             * http://www.example.com/nbib#Techreport 
             * 
             * A report published by a school or other institution, usually 
             * numbered within a series. 
             */
            QUrl Techreport();

            /**
             * http://www.example.com/nbib#Thesis 
             * 
             * A general thesis. 
             */
            QUrl Thesis();

            /**
             * http://www.example.com/nbib#Unpublished 
             * 
             * A document having an author and title, but not formally published. 
             */
            QUrl Unpublished();

            /**
             * http://www.example.com/nbib#Webpage 
             * 
             * Reference a webpage in general (only one site of the complete 
             * Website) 
             */
            QUrl Webpage();

            /**
             * http://www.example.com/nbib#Website 
             * 
             * Reference the complete website in which several webpages can 
             * be found 
             */
            QUrl Website();

            /**
             * http://www.example.com/nbib#accepted 
             * 
             * Accepted for publication after peer reviewing. 
             */
            QUrl accepted();

            /**
             * http://www.example.com/nbib#affirmedBy 
             * 
             * A legal decision that affirms a ruling. 
             */
            QUrl affirmedBy();

            /**
             * http://www.example.com/nbib#applicationNumber 
             * 
             * The application number of a patent 
             */
            QUrl applicationNumber();

            /**
             * http://www.example.com/nbib#archive 
             * 
             * The archive where the publication can be found, see also nbib::archivelocation 
             */
            QUrl archive();

            /**
             * http://www.example.com/nbib#archiveLocation 
             * 
             * The location where the publication can be found in the archive 
             * specified via nbib::archive 
             */
            QUrl archiveLocation();

            /**
             * http://www.example.com/nbib#arguedDate 
             * 
             * The date on which a legal case is argued before a court. 
             */
            QUrl arguedDate();

            /**
             * http://www.example.com/nbib#article 
             * 
             * Connects an article to its collection (article in Newspaper, 
             * Proceedings, Journalissue) 
             */
            QUrl article();

            /**
             * http://www.example.com/nbib#assignee 
             * 
             * The assignee of an patent 
             */
            QUrl assignee();

            /**
             * http://www.example.com/nbib#attorneyAgent 
             * 
             * A attorney agent related to a patent 
             */
            QUrl attorneyAgent();

            /**
             * http://www.example.com/nbib#chapterNumber 
             * 
             * The number of the Chapter for example 1.2, II etc 
             */
            QUrl chapterNumber();

            /**
             * http://www.example.com/nbib#citeKey 
             * 
             * Used to identify the reference. 
             */
            QUrl citeKey();

            /**
             * http://www.example.com/nbib#citedReference 
             * 
             * Links all cited references from one publication. Good way to 
             * find new sources to read 
             */
            QUrl citedReference();

            /**
             * http://www.example.com/nbib#coSponsor 
             * 
             * The co-sponsor [of a bill] 
             */
            QUrl coSponsor();

            /**
             * http://www.example.com/nbib#codeNumber 
             * 
             * The code number of an CodeOfLaw 
             */
            QUrl codeNumber();

            /**
             * http://www.example.com/nbib#codeOfLaw 
             * 
             * The code of law the legislation is contained in 
             */
            QUrl codeOfLaw();

            /**
             * http://www.example.com/nbib#collection 
             * 
             * The collection this article was published in (for example a 
             * Journalissue, Newspaper or Proceedings 
             */
            QUrl collection();

            /**
             * http://www.example.com/nbib#commenter 
             * 
             * The commenter of a Publication. Mostly used for Blogs, Forum 
             * posts 
             */
            QUrl commenter();

            /**
             * http://www.example.com/nbib#contributor 
             * 
             * The contributor of a Publication 
             */
            QUrl contributor();

            /**
             * http://www.example.com/nbib#counsel 
             * 
             * The counsel related to a Legal document 
             */
            QUrl counsel();

            /**
             * http://www.example.com/nbib#courtReporter 
             * 
             * Links case document to the collection of legal cases it belongs 
             * to. 
             */
            QUrl courtReporter();

            /**
             * http://www.example.com/nbib#documentPart 
             * 
             * Connects a publication to a part of it (for example a chapter 
             * or quoted passage) 
             */
            QUrl documentPart();

            /**
             * http://www.example.com/nbib#documentPartOf 
             * 
             * Connects a part of a document to the publication it is from 
             */
            QUrl documentPartOf();

            /**
             * http://www.example.com/nbib#documentStatus 
             * 
             * The status of the publication of a document. 
             */
            QUrl documentStatus();

            /**
             * http://www.example.com/nbib#draft 
             * 
             * Document drafted. 
             */
            QUrl draft();

            /**
             * http://www.example.com/nbib#edition 
             * 
             * The edition of a book, long form (such as 'first' or 'second'). 
             */
            QUrl edition();

            /**
             * http://www.example.com/nbib#eprint 
             * 
             * A specification of an electronic publication, often a preprint 
             * or a technical report. 
             */
            QUrl eprint();

            /**
             * http://www.example.com/nbib#event 
             * 
             * The event this publication was shown, represented, published 
             * etc 
             */
            QUrl event();

            /**
             * http://www.example.com/nbib#eventPublication 
             * 
             * the publications that fornm the result of an event 
             */
            QUrl eventPublication();

            /**
             * http://www.example.com/nbib#filingDate 
             * 
             * The date when the patent was filed, the issue date is the same 
             * as the publication date 
             */
            QUrl filingDate();

            /**
             * http://www.example.com/nbib#forthcoming 
             * 
             * Document to be published. 
             */
            QUrl forthcoming();

            /**
             * http://www.example.com/nbib#history 
             * 
             * The history of a legal document 
             */
            QUrl history();

            /**
             * http://www.example.com/nbib#inSeries 
             * 
             * The series the publication was published in. (for example a 
             * series of books, a series of lecture notes, a journal series 
             */
            QUrl inSeries();

            /**
             * http://www.example.com/nbib#isPublicationOf 
             * 
             * Links a publication to its document. 
             */
            QUrl isPublicationOf();

            /**
             * http://www.example.com/nbib#isbn 
             * 
             * The International Standard Book Number. 
             */
            QUrl isbn();

            /**
             * http://www.example.com/nbib#issn 
             * 
             * The International Standard Serial Number. Used to identify 
             * a series of publications. 
             */
            QUrl issn();

            /**
             * http://www.example.com/nbib#lccn 
             * 
             * The Library of Congress Call Number. 
             */
            QUrl lccn();

            /**
             * http://www.example.com/nbib#legal 
             * 
             * Legal document. 
             */
            QUrl legal();

            /**
             * http://www.example.com/nbib#legalCase 
             * 
             * Backlinks all legal cases from the court reporter it is from 
             */
            QUrl legalCase();

            /**
             * http://www.example.com/nbib#legalStatus 
             * 
             * The legal status of this patent 
             */
            QUrl legalStatus();

            /**
             * http://www.example.com/nbib#legislation 
             * 
             * links to a legislation (Bill, Statute) in a Code of Law 
             */
            QUrl legislation();

            /**
             * http://www.example.com/nbib#libraryCatalog 
             * 
             * The library catalog information where the publication can 
             * be found 
             */
            QUrl libraryCatalog();

            /**
             * http://www.example.com/nbib#mapScale 
             * 
             * The scale of a map 
             */
            QUrl mapScale();

            /**
             * http://www.example.com/nbib#mrNumber 
             * 
             * The Mathematical Reviews number. 
             */
            QUrl mrNumber();

            /**
             * http://www.example.com/nbib#nonPeerReviewed 
             * 
             * A document that is not peer reviewed. 
             */
            QUrl nonPeerReviewed();

            /**
             * http://www.example.com/nbib#number 
             * 
             * The '(issue) number' of a journal, magazine, or tech-report, 
             * pr the publicl aw Number of a Statute or the number of a patent 
             * etc 
             */
            QUrl number();

            /**
             * http://www.example.com/nbib#numberOfPages 
             * 
             * The number of pages contained in the publication. 
             */
            QUrl numberOfPages();

            /**
             * http://www.example.com/nbib#numberOfVolumes 
             * 
             * The number of volumes that exists for a publication. 
             */
            QUrl numberOfVolumes();

            /**
             * http://www.example.com/nbib#organization 
             * 
             * The organization that held the conference where the proceedings 
             * comes from or that is otherwise responsible for the publication 
             */
            QUrl organization();

            /**
             * http://www.example.com/nbib#pageEnd 
             * 
             * Ending page number of the chapter 
             */
            QUrl pageEnd();

            /**
             * http://www.example.com/nbib#pageStart 
             * 
             * Starting page number of the chapter 
             */
            QUrl pageStart();

            /**
             * http://www.example.com/nbib#pages 
             * 
             * Page numbers, separated either by commas or double-hyphens. 
             */
            QUrl pages();

            /**
             * http://www.example.com/nbib#patentReferences 
             * 
             * The references for this patent 
             */
            QUrl patentReferences();

            /**
             * http://www.example.com/nbib#peerReviewed 
             * 
             * The process by which articles are chosen to be included in a refereed 
             * journal. An editorial board consisting of experts in the same 
             * field as the author review the article and decide if it is authoritative 
             * enough for publication. 
             */
            QUrl peerReviewed();

            /**
             * http://www.example.com/nbib#priorityNumbers 
             * 
             * The priority number of a patent 
             */
            QUrl priorityNumbers();

            /**
             * http://www.example.com/nbib#pubMed 
             * 
             * The PubMed ID. 
             */
            QUrl pubMed();

            /**
             * http://www.example.com/nbib#publicLawNumber 
             * 
             * The public law number of the statute 
             */
            QUrl publicLawNumber();

            /**
             * http://www.example.com/nbib#publication 
             * 
             * The publication used for this reference 
             */
            QUrl publication();

            /**
             * http://www.example.com/nbib#publicationMethod 
             * 
             * How it was published, if the publishing method is nonstandard. 
             */
            QUrl publicationMethod();

            /**
             * http://www.example.com/nbib#publicationType 
             * 
             * The type of tech-report or other kind of Publication, for example, 
             * 'Research Note'. 
             */
            QUrl publicationType();

            /**
             * http://www.example.com/nbib#published 
             * 
             * Published document. 
             */
            QUrl published();

            /**
             * http://www.example.com/nbib#publishedAs 
             * 
             * Links a document to its publication information. 
             */
            QUrl publishedAs();

            /**
             * http://www.example.com/nbib#reference 
             * 
             * All references of a publication. Used to quickly get all different 
             * references (different quotes, chapters etc) 
             */
            QUrl reference();

            /**
             * http://www.example.com/nbib#referencedPart 
             * 
             * The specific part of the document of a reference. Used to reference 
             * a chapter or quote a small portion of a publication 
             */
            QUrl referencedPart();

            /**
             * http://www.example.com/nbib#rejected 
             * 
             * Rejected for publication after peer reviewing. 
             */
            QUrl rejected();

            /**
             * http://www.example.com/nbib#reviewedAuthor 
             * 
             * The person who reviewed a Publication 
             */
            QUrl reviewedAuthor();

            /**
             * http://www.example.com/nbib#seriesOf 
             * 
             * The publication this series belongs to 
             */
            QUrl seriesOf();

            /**
             * http://www.example.com/nbib#shortTitle 
             * 
             * The short version of the title 
             */
            QUrl shortTitle();

            /**
             * http://www.example.com/nbib#translationOf 
             * 
             * Relates a translated document to the original document. 
             */
            QUrl translationOf();

            /**
             * http://www.example.com/nbib#translator 
             * 
             * The translator of a Publication 
             */
            QUrl translator();

            /**
             * http://www.example.com/nbib#unpublished 
             * 
             * Unpublished document. 
             */
            QUrl unpublished();

            /**
             * http://www.example.com/nbib#volume 
             * 
             * The volume of a journal or multi-volume book. 
             */
            QUrl volume();

            /**
             * http://www.semanticdesktop.org/ontologies/2007/01/19/nie#abstract 
             * 
             * An abstract of the work. 
             */
            QUrl abstract();

            /**
             * http://www.semanticdesktop.org/ontologies/2007/01/19/nie#publicationDate 
             * 
             * The date of the publication 
             */
            QUrl publicationDate();

            /**
             * http://www.semanticdesktop.org/ontologies/2007/03/22/nco#editor 
             * 
             * The name(s) of the editor(s). 
             */
            QUrl editor();

            /**
             * http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#doi 
             * 
             * The Digital object identifier. see http://www.doi.org/ for 
             * more information 
             */
            QUrl doi();
        }
    }
}

#endif
