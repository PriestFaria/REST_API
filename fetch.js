function getRubleExchangeRateDynamic() {
  // Please adjust valutaCode as required.
  // The ValutaCode for the Russian ruble could be something like 'R01235' for USD/RUB, for example.
  let valutaCode = 'R01235';

  // Setting the dates for the entire year of 2023
  let fromDate = '2022-02-01';
  let toDate = '2022-02-01';

  // Your XML payload using template strings for the dates and valutaCode
  const requestBody = `<?xml version="1.0" encoding="utf-8"?>
<soap12:Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:soap12="http://www.w3.org/2003/05/soap-envelope">
  <soap12:Body>
    <GetCursDynamic xmlns="http://web.cbr.ru/">
      <FromDate>${fromDate}</FromDate>
      <ToDate>${toDate}</ToDate>
      <ValutaCode>${valutaCode}</ValutaCode>
    </GetCursDynamic>
  </soap12:Body>
</soap12:Envelope>`;

  fetch('https://www.cbr.ru/DailyInfoWebServ/DailyInfo.asmx', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/soap+xml; charset=utf-8'
    },
    body: requestBody
  })
      .then(response => {
        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        return response.text();
      })
      .then(data => {
        // The XML response is in the 'data' variable. You can process it as required.
        console.log(data);
      })
      .catch(error => {
        console.error('Fetch error:', error);
      });
  console.log(requestBody)
}

// Example usage:
getRubleExchangeRateDynamic();