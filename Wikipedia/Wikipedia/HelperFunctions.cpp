//
// HelperFunctions.cpp
// Implementation of the HelperFunctions class
// 
// See HelperFunctions.h for details
//

#include "pch.h"
#include "HelperFunctions.h"

using namespace std;
using namespace Wikipedia;

using namespace Platform;
using namespace Windows::Storage;
using namespace Windows::Web::Http;
using namespace Windows::Web::Http::Filters;
using namespace Windows::Web::Http::Headers;
using namespace Windows::UI::Popups;

// Creates a HTTP client and adds headers
HttpClient^ HelperFunctions::CreateHttpClient() {
	auto loader = ref new Windows::ApplicationModel::Resources::ResourceLoader();
	auto values = ApplicationData::Current->LocalSettings->Values;

	HttpBaseProtocolFilter^ filter = ref new HttpBaseProtocolFilter();

	HttpClient^ httpClient = ref new HttpClient(filter);

	// Add a user-agent header
	HttpRequestHeaderCollection ^headers = httpClient->DefaultRequestHeaders;
	headers->UserAgent->ParseAdd(loader->GetString("UserAgent"));
	
	return httpClient;
}

// Displays a popup message
void HelperFunctions::ErrorMessage(Platform::String^ message) {
	MessageDialog^ messageDialog = ref new MessageDialog(message);
	messageDialog->ShowAsync();
}


// Helper functions to convert underscores into space
wstring HelperFunctions::UnderscoreToSpace(wstring s) {
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == '_')
			s[i] = ' ';
	}
	return s;
}

String^ HelperFunctions::UnderscoreToSpace(String^ s) {
	wstring sStr = s->Data();
	sStr = UnderscoreToSpace(sStr);
	return ref new String(sStr.c_str());
}

// errrr.... does nothing
void HelperFunctions::DoNothing(IUICommand^ command){
}


String^ HelperFunctions::ProcessImages(String^ sectionStr) {
	wstring section = sectionStr->Data();

	wstring imgSrc = L"src=\"//";
	wstring newImgSrc = L"src=\"https://";

	int pos = section.find(imgSrc);

	while (pos != wstring::npos) {
		section = section.replace(pos, imgSrc.size(), newImgSrc);
		pos = section.find(imgSrc);
	}

	/*wstring infoboxStart = L"<table class=\"infobox vevent haudio\" cellspacing=\"3\" style=\"border-spacing:3px;width:22em;\">";
	pos = section.find(infoboxStart);

	if (pos != wstring::npos) {
	wstring newInfoboxStart = L"<table class=\"infobox vevent haudio\" cellspacing=\"3\" style=\"border-spacing:3px;\">";
	section = section.replace(pos, infoboxStart.size(), newInfoboxStart);
	}*/

	return ref new String(section.c_str());
}

String^ HelperFunctions::ProcessSection(String^ sectionStr, int id, int level) {
	wstring section = sectionStr->Data();

	String^ openingTagP = (L"<h" + level + L">");
	wstring openingTagW = openingTagP->Data();
	String^ closingTagP = (L"</h" + level + L">");
	wstring closingTagW = closingTagP->Data();

	//int pos = section.find(openingTagW);
	// This ain't doing shit right now
	//if (id == 1) {
	//	String^ divCode = /*L"</div>" +*/ openingTagP;
	//	section = section.replace(pos, 4, divCode->Data());
	//}

	// 'cos the extra spans fuck with main find and replace
	wstring openBracket = L"<span class=\"mw-editsection-bracket\">[</span>";
	int pos = section.find(openBracket);
	if (pos != wstring::npos)
		section = section.replace(pos, openBracket.size(), L"");

	wstring closeBracket = L"<span class=\"mw-editsection-bracket\">]</span>";
	pos = section.find(closeBracket);
	if (pos != wstring::npos)
		section = section.replace(pos, closeBracket.size(), L"");

	wstring divider = L"<span class=\"mw-editsection-divider\"> | </span>";
	pos = section.find(divider);
	if (pos != wstring::npos)
		section = section.replace(pos, divider.size(), L"");

	wstring editText = L"<span class=\"mw-editsection\">";
	pos = section.find(editText);
	int pos2 = section.find(L"</span>", pos);
	if (pos != wstring::npos && pos2 != wstring::npos) {
		wstring editImage = ("<a href=\"about:/w/index.php?title=meh&action=edit&section=" + id + "\"><img class=\"editIcon\" id=\"editsection" + id + "\" src=\"data:image/png; base64, iVBORw0KGgoAAAANSUhEUgAAAFAAAABQCAMAAAC5zwKfAAAANlBMVEUAAAD///9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW+FiFraAAAAEXRSTlMAABAgMEBQYHCAkKCwwNDg8InUWv0AAAGfSURBVHhe7djBjsIgFIXhOwjFeltK3/9lJ4J6SG5dHGgyyaRn1dUXlb8slJ+T9//AC7zACxRiLu37EqXsDPCW9ue2cBLo8/7a/RQwFK9OTwDj3k6Hwbk6Mb/EeRDUojyaHzKMgG4pRmy/ehoAXcklT3h+buoGffU8vDJlQJtfuuG5busEY/Xc28N4EPkp7EHwUaODjSkLIr8Iu/VIELkE2Bj9psDzSBuLQoLIxX1sLEfhQHirQ9rwvLAgcoHdejx4b3IJ8PDOcCBysfklJzToipc9Piu2OKFA5LL5o/xUhAZ9Qi7W40Dkspj8as48OGXkYj0ORH73w/yC8OCMXA5y5sE2lwgPh86Bbi3h+i8506Brc3kYjwb9VkL7lh8N+jaXxXg0GBGayW8WCoSXq+dNzjyIXGx+UXhQm1yCyZkHA8JAfvBosF6h21RvU3vb86B+4lCTcxf4rkTV3PZ9IAiTcxcYvnm94P3Qi9IN6rHXDybL5SAD4IHnZQAMxtu8jIA4E+Q8BKr1xkCcybrOcwhOBsE3JOT+4G+WC7zAC7zAXxMIT+qGYJfyAAAAAElFTkSuQmCC\" alt=\"edit\" width=\"128\" height = \"128\"/></a>")->Data();
		section = section.replace(pos, pos2 - pos + 7, editImage);
	}



	/*

	wstring editText = L"Edit</a>";
	pos = section.find(editText);
	if (pos != wstring::npos) {
		wstring editImage = ("<img class=\"editIcon\" id=\"editsection" + id + "\" src=\"data:image/png; base64, iVBORw0KGgoAAAANSUhEUgAAAFAAAABQCAMAAAC5zwKfAAAANlBMVEUAAAD///9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW+FiFraAAAAEXRSTlMAABAgMEBQYHCAkKCwwNDg8InUWv0AAAGfSURBVHhe7djBjsIgFIXhOwjFeltK3/9lJ4J6SG5dHGgyyaRn1dUXlb8slJ+T9//AC7zACxRiLu37EqXsDPCW9ue2cBLo8/7a/RQwFK9OTwDj3k6Hwbk6Mb/EeRDUojyaHzKMgG4pRmy/ehoAXcklT3h+buoGffU8vDJlQJtfuuG5busEY/Xc28N4EPkp7EHwUaODjSkLIr8Iu/VIELkE2Bj9psDzSBuLQoLIxX1sLEfhQHirQ9rwvLAgcoHdejx4b3IJ8PDOcCBysfklJzToipc9Piu2OKFA5LL5o/xUhAZ9Qi7W40Dkspj8as48OGXkYj0ORH73w/yC8OCMXA5y5sE2lwgPh86Bbi3h+i8506Brc3kYjwb9VkL7lh8N+jaXxXg0GBGayW8WCoSXq+dNzjyIXGx+UXhQm1yCyZkHA8JAfvBosF6h21RvU3vb86B+4lCTcxf4rkTV3PZ9IAiTcxcYvnm94P3Qi9IN6rHXDybL5SAD4IHnZQAMxtu8jIA4E+Q8BKr1xkCcybrOcwhOBsE3JOT+4G+WC7zAC7zAXxMIT+qGYJfyAAAAAElFTkSuQmCC\" alt=\"edit\" width=\"128\" height = \"128\"/></a>")->Data();
		section = section.replace(pos, editText.size(), editImage);
	}

	editText = L"edit</a>";
	pos = section.find(editText);
	if (pos != wstring::npos) {
		wstring editImage = ("<img class=\"editIcon\" id=\"editsection" + id + "\" src=\"data:image/png; base64, iVBORw0KGgoAAAANSUhEUgAAAFAAAABQCAMAAAC5zwKfAAAANlBMVEUAAAD///9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW+FiFraAAAAEXRSTlMAABAgMEBQYHCAkKCwwNDg8InUWv0AAAGfSURBVHhe7djBjsIgFIXhOwjFeltK3/9lJ4J6SG5dHGgyyaRn1dUXlb8slJ+T9//AC7zACxRiLu37EqXsDPCW9ue2cBLo8/7a/RQwFK9OTwDj3k6Hwbk6Mb/EeRDUojyaHzKMgG4pRmy/ehoAXcklT3h+buoGffU8vDJlQJtfuuG5busEY/Xc28N4EPkp7EHwUaODjSkLIr8Iu/VIELkE2Bj9psDzSBuLQoLIxX1sLEfhQHirQ9rwvLAgcoHdejx4b3IJ8PDOcCBysfklJzToipc9Piu2OKFA5LL5o/xUhAZ9Qi7W40Dkspj8as48OGXkYj0ORH73w/yC8OCMXA5y5sE2lwgPh86Bbi3h+i8506Brc3kYjwb9VkL7lh8N+jaXxXg0GBGayW8WCoSXq+dNzjyIXGx+UXhQm1yCyZkHA8JAfvBosF6h21RvU3vb86B+4lCTcxf4rkTV3PZ9IAiTcxcYvnm94P3Qi9IN6rHXDybL5SAD4IHnZQAMxtu8jIA4E+Q8BKr1xkCcybrOcwhOBsE3JOT+4G+WC7zAC7zAXxMIT+qGYJfyAAAAAElFTkSuQmCC\" alt=\"edit\" width=\"128\" height = \"128\"/></a>")->Data();
		section = section.replace(pos, editText.size(), editImage);
	}
	*/
	editText = L"<a href=\"#/editor/";
	pos = section.find(editText);
	pos2 = section.find(L"</a>", pos);
	if (pos != wstring::npos && pos2 != wstring::npos) {
		wstring editImage = ("<a href=\"about:/w/index.php?title=meh&action=edit&section=" + id + "\"><img class=\"editIcon\" id=\"editsection" + id + "\" src=\"data:image/png; base64, iVBORw0KGgoAAAANSUhEUgAAAFAAAABQCAMAAAC5zwKfAAAANlBMVEUAAAD///9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW9sbW+FiFraAAAAEXRSTlMAABAgMEBQYHCAkKCwwNDg8InUWv0AAAGfSURBVHhe7djBjsIgFIXhOwjFeltK3/9lJ4J6SG5dHGgyyaRn1dUXlb8slJ+T9//AC7zACxRiLu37EqXsDPCW9ue2cBLo8/7a/RQwFK9OTwDj3k6Hwbk6Mb/EeRDUojyaHzKMgG4pRmy/ehoAXcklT3h+buoGffU8vDJlQJtfuuG5busEY/Xc28N4EPkp7EHwUaODjSkLIr8Iu/VIELkE2Bj9psDzSBuLQoLIxX1sLEfhQHirQ9rwvLAgcoHdejx4b3IJ8PDOcCBysfklJzToipc9Piu2OKFA5LL5o/xUhAZ9Qi7W40Dkspj8as48OGXkYj0ORH73w/yC8OCMXA5y5sE2lwgPh86Bbi3h+i8506Brc3kYjwb9VkL7lh8N+jaXxXg0GBGayW8WCoSXq+dNzjyIXGx+UXhQm1yCyZkHA8JAfvBosF6h21RvU3vb86B+4lCTcxf4rkTV3PZ9IAiTcxcYvnm94P3Qi9IN6rHXDybL5SAD4IHnZQAMxtu8jIA4E+Q8BKr1xkCcybrOcwhOBsE3JOT+4G+WC7zAC7zAXxMIT+qGYJfyAAAAAElFTkSuQmCC\" alt=\"edit\" width=\"128\" height = \"128\"/></a>")->Data();
		section = section.replace(pos, pos2 - pos + 4, editImage);
	}


	pos = section.find(closingTagW);
	if (pos != wstring::npos) {
		String^ downImage = "<img id=\"section" + id + "img\" src=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAMgAAADICAYAAACtWK6eAAAABmJLR0QA/wD/AP+gvaeTAAADKElEQVR4nO3duWoVURzH8Z/auKCgiGAs7HwBn0UEwWdwi4qVI4gWWtn5IDZWggsi4gLi9gZW9oLGIlxRk8zdZjl35vOBU+d/5uR7z500SQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAOB/e5LcSPIqyZskd5Ps7XWi4duf5H6St0leJllPsrvXidjW/iSPk2z8t94kOdrjXEN2LMn7bH3mj+KDqSiHkjzP1oOarM9Jjvc23TCdSPI1Oz/zp0kO9jYdfxxK8iI7H5RImjctDpEU4nCS15l+UCJpzqxxTNaziKQXRzJfHCJZ3rxxTNbzbN70dORANj+Z5j2oyXoXL+7zOpbkQxZ/5k+S7Ot66LF6mMUPSiTzWzaOyXrQ9eBjdDDJzyx/WL5uzWbRr1XbrR/x59/WnUozh+Umma6pm+PvdbLTHYzQniTfI5K2tRHHtyS7utzEWJ1Pswcnkn+1EcdGkrNdbmLsLqX5A/RO0uw7x2T9SnKhy02w6XJE0iRxDJBImiGOARPJcsQxAiJZjDhGRCTzaSuOi11ugvmIZDbiGDGR1Gsjjo2IY6VciUi2Iw7+EMm/xMEWItkkDnY09kjEwVRjjUQczGxskYiDuY0lEnGwsPUMOxJxsLShRtJWHJe63ARlGFok4qBxQ4lEHLRm1SMRB61b1UjEQWdWLRJx0LmrWY1IxEFvSo+krTguNzQfI1BqJOKgGKVFIg6KU0ok4qBYfUciDop3Lf1EIg5WRteRrEUcrJiuIllL8qWFn3WlsScBO2g7EnGw8tqI5FOS0xEHA3Enzf8iN/UPSv9eVUv7h6nauEncHAzK9fQfgjgoWmmRiIPilBKJOCjWzfQbx3r7W4TlVBEH1KoiDqhVRRxQq4o4oFYVcUCtKuKAWlWaieNqx3NDZ6qIA2pVEQfUuhVxQK1ZIxEHozUtEnEweveyfRy3+xwKSnIuycckP5J8SHKm33EAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAGAEfgM0pZJdBuQicgAAAABJRU5ErkJggg==\" alt=\"show\" width=\"128\" height = \"128\"/>";
		String^ showCode = "<span class=\"mw-showsection\"><a href=\"#section" + id + "\" onclick =\"show_hide('section" + id + L"');\" title=\"Show/hide section\">" + downImage + "</a></span>" + closingTagP + "<a id=\"sectionLink" + id + "\"\><div class=\"section\" id=\"section" + id + L"\">";
		section = section.replace(pos, 5, showCode->Data());
	}

	return ref new String(section.c_str());
}

String^ HelperFunctions::ProcessContent(String^ contentStr, String^ title) {
	String^ headerStart = "<!DOCTYPE html><html><head>";
	String^ script = "<script type=\"text/javascript\">	function show_hide(id) {	var e = document.getElementById(id);	var i = document.getElementById(id+\"img\"); 	var j = document.getElementById(\"edit\"+id); 		if (e.style.display == 'block'){	e.style.display = 'none';	j.style.display='none';	i.src = \"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAMgAAADICAYAAACtWK6eAAAABmJLR0QA/wD/AP+gvaeTAAADKElEQVR4nO3duWoVURzH8Z/auKCgiGAs7HwBn0UEwWdwi4qVI4gWWtn5IDZWggsi4gLi9gZW9oLGIlxRk8zdZjl35vOBU+d/5uR7z500SQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAOB/e5LcSPIqyZskd5Ps7XWi4duf5H6St0leJllPsrvXidjW/iSPk2z8t94kOdrjXEN2LMn7bH3mj+KDqSiHkjzP1oOarM9Jjvc23TCdSPI1Oz/zp0kO9jYdfxxK8iI7H5RImjctDpEU4nCS15l+UCJpzqxxTNaziKQXRzJfHCJZ3rxxTNbzbN70dORANj+Z5j2oyXoXL+7zOpbkQxZ/5k+S7Ot66LF6mMUPSiTzWzaOyXrQ9eBjdDDJzyx/WL5uzWbRr1XbrR/x59/WnUozh+Umma6pm+PvdbLTHYzQniTfI5K2tRHHtyS7utzEWJ1Pswcnkn+1EcdGkrNdbmLsLqX5A/RO0uw7x2T9SnKhy02w6XJE0iRxDJBImiGOARPJcsQxAiJZjDhGRCTzaSuOi11ugvmIZDbiGDGR1Gsjjo2IY6VciUi2Iw7+EMm/xMEWItkkDnY09kjEwVRjjUQczGxskYiDuY0lEnGwsPUMOxJxsLShRtJWHJe63ARlGFok4qBxQ4lEHLRm1SMRB61b1UjEQWdWLRJx0LmrWY1IxEFvSo+krTguNzQfI1BqJOKgGKVFIg6KU0ok4qBYfUciDop3Lf1EIg5WRteRrEUcrJiuIllL8qWFn3WlsScBO2g7EnGw8tqI5FOS0xEHA3Enzf8iN/UPSv9eVUv7h6nauEncHAzK9fQfgjgoWmmRiIPilBKJOCjWzfQbx3r7W4TlVBEH1KoiDqhVRRxQq4o4oFYVcUCtKuKAWlWaieNqx3NDZ6qIA2pVEQfUuhVxQK1ZIxEHozUtEnEweveyfRy3+xwKSnIuycckP5J8SHKm33EAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAGAEfgM0pZJdBuQicgAAAABJRU5ErkJggg==\";		i.alt=\"show\";			}else{ 				e.style.display = 'block';			j.style.display='inline';i.src=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAMgAAADICAYAAACtWK6eAAAABmJLR0QA/wD/AP+gvaeTAAADCElEQVR4nO3dP68VRRzH4S+5gUSq2/gGLLUz1lRA5xswsbGwIBQkvgPfgCGBmNhBKKGislALLW0IPUQq6I2BRIECJkaul3P/zM7M7jxPsu2e35w9n8zuSU5OAgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAMBkvk7yW5KHSb5Pst93HBjDXpJbSV69czxK8lHHuaC7s0nu5mAc5Xia5ONu00FHu+IQCdM6ahwiYTrHjUMkTOOkcYiEzTttHCJhs2rFIRI2p3YcImEzlopDJKze0nGIhNVqFYdIWJ3WcYiE1egVh0gYXu84RMKwRolDJAxntDhEwjBGjUMkdDd6HCKhm70kd1L/w/xTkm+SvKx83ifx810aWWrnuJ/k3NvX+CrJP5XPbydhcS3iKETCqrSMoxAJq9AjjkIkDK1nHIVIGNIIcRQiYSgjxVGIhCGMGEchEroaOY5CJHSxhjgKkdDUmuIoREITa4yjEAmLOpvkXtYZRyESFrGFOAqRUNWW4ihEQhVbjKMQCaey5TgKkXAiM8RRiIRjmSmOYqlIPmm5CJY3YxyFSHivmeMoRML/Ese/RMJ/iOMgkZBEHO8jksmJYzeRTEocRyeSyYjj+EQyCXGcnEg2ThynJ5KNEkc9ItkYcdQnko0Qx3JEsgHXUz+On5Ocb7mIgV1N/T/xeZxkv+UiZvVZ6l88cRy0RCTfNV3BpK5FHK3UjuT3tuPP6cvUu2CeOXar+UzyY+PZp/Rhkmexc7RUayf5ovXgs7qY5HnE0dJpI/mh/chzu5Dkz7itaumkt1s3kpzpMO/0Lib5K+Jo6biRiKOzSzlaJOKo56iR3Iw4hrArEnHUtysScQzm8yQvcvBC/RIP5Es57MH9dpK9jnNxiE+T/Jrk77z5Kvjb2DmWdjnJg7wJ5Y8kV2LnGN4HvQeYkPccAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA6OE1lY+WZrWOLe8AAAAASUVORK5CYII=\";	i.alt=\"hide\";	}} 		function anchor(id) {try{window.location.hash=id;document.getElementById(\"testzone\").innerHTML = document.URL;}catch(err){/*document.getElementById(\"section0\").innerHTML = document.URL;*/}} 	function anchorNew(id) {	window.scroll(0,findPos(document.getElementById(id))+50); }	function findPos(obj) {   var curtop = 0;    if (obj.offsetParent) {        do {            curtop += obj.offsetTop;        } while (obj = obj.offsetParent);    return [curtop];    }} 	function refresh() { window.location.hash='';window.location.reload();}	function openSection(id) {var e = document.getElementById(id);var i = document.getElementById(id+\"img\"); var j = document.getElementById(\"edit\"+id); if (e.style.display != 'block'){e.style.display = 'block';	j.style.display='inline';i.src=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAMgAAADICAYAAACtWK6eAAAABmJLR0QA/wD/AP+gvaeTAAADCElEQVR4nO3dP68VRRzH4S+5gUSq2/gGLLUz1lRA5xswsbGwIBQkvgPfgCGBmNhBKKGislALLW0IPUQq6I2BRIECJkaul3P/zM7M7jxPsu2e35w9n8zuSU5OAgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAMBkvk7yW5KHSb5Pst93HBjDXpJbSV69czxK8lHHuaC7s0nu5mAc5Xia5ONu00FHu+IQCdM6ahwiYTrHjUMkTOOkcYiEzTttHCJhs2rFIRI2p3YcImEzlopDJKze0nGIhNVqFYdIWJ3WcYiE1egVh0gYXu84RMKwRolDJAxntDhEwjBGjUMkdDd6HCKhm70kd1L/w/xTkm+SvKx83ifx810aWWrnuJ/k3NvX+CrJP5XPbydhcS3iKETCqrSMoxAJq9AjjkIkDK1nHIVIGNIIcRQiYSgjxVGIhCGMGEchEroaOY5CJHSxhjgKkdDUmuIoREITa4yjEAmLOpvkXtYZRyESFrGFOAqRUNWW4ihEQhVbjKMQCaey5TgKkXAiM8RRiIRjmSmOYqlIPmm5CJY3YxyFSHivmeMoRML/Ese/RMJ/iOMgkZBEHO8jksmJYzeRTEocRyeSyYjj+EQyCXGcnEg2ThynJ5KNEkc9ItkYcdQnko0Qx3JEsgHXUz+On5Ocb7mIgV1N/T/xeZxkv+UiZvVZ6l88cRy0RCTfNV3BpK5FHK3UjuT3tuPP6cvUu2CeOXar+UzyY+PZp/Rhkmexc7RUayf5ovXgs7qY5HnE0dJpI/mh/chzu5Dkz7itaumkt1s3kpzpMO/0Lib5K+Jo6biRiKOzSzlaJOKo56iR3Iw4hrArEnHUtysScQzm8yQvcvBC/RIP5Es57MH9dpK9jnNxiE+T/Jrk77z5Kvjb2DmWdjnJg7wJ5Y8kV2LnGN4HvQeYkPccAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA6OE1lY+WZrWOLe8AAAAASUVORK5CYII=\";	i.alt=\"hide\";}}	</script>";
	String^ style = "<style>.thumbcaption { width:200%;} .thumb { margin-left:25%; }.thumbcaption{ font-size:200%;}.thumbinner .image img {width:200%; height:200%;}.vertical-navbox.nowraplinks li { font-size:100%;}.wikitable, .wikitable th, .wikitable td{  border: 1px solid black;}.navbox li {font-size: 75%;}  .plainlinks.hlist.navbar.mini { display:none;} .navbox li { display:inline; margin-left:10px;} .infobox li { font-size:16px;color:red; } body {font-size:16px;} li{font-size:32px;} .hatnote{font-size:10px;font-style:italic;} .editIcon { display:none;} .infobox { border: 1px solid #aaa;  background-color: #f9f9f9; color: black;  text-align: left;  line-height: 1.5em; 	width: 100%; margin-left:15%; font-size: 28px; float:center; width:200%;}.section {display: none;} .navbox {font-size: 24px;} #mp-tfa {font-size:32px;}</style>";
	String^ headerClose = "</head><body><div id=\"section0\"><h1>" + HelperFunctions::UnderscoreToSpace(title) + "</h1>";
	String^ footer = "</body></html>";
	String^ content = headerStart + script + style + headerClose + contentStr + footer;

	return content;
}
