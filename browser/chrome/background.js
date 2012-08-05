/*
 * Copyright 2012 Jörg Ehrichs <joerg.ehrichs@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// called whenever the url of a tab changes
function checkForValidUrl(tabId, changeInfo, tab) {

    npapi = document.getElementById("pluginObj");
    urlCheck = npapi.checkUrl(tab.url);

    console.log( "Url supported by a plugin?" );
    console.log( urlCheck );

    if( urlCheck ) {
        chrome.pageAction.show(tabId);
    }
    else {
        chrome.pageAction.hide(tabId);
    }
};

// called when the user left clicks on the icon
function fetchAndSaveMetaData(tab) {
    console.log("fetchAndSaveMetaData");

    npapi = document.getElementById("pluginObj");

    console.log( npapi.fetchMetaData(tab.url) );
}

// Listen for any changes to the URL of any tab.
chrome.tabs.onUpdated.addListener(checkForValidUrl);
chrome.pageAction.onClicked.addListener(fetchAndSaveMetaData);