(() => {
  'use strict'

  const updateGpioStatuses = () => {
    const req = {}

    document.querySelectorAll('[data-gpio]').forEach(element => {
      fetch('/gpio/' + element.dataset.gpio).then((response) => {
        const resp = response.json();
        element.setAttribute('value', resp.value);
        element.removeAttribute('disabled');

        console.log(resp);
      }).then(data => 
        console.log(data)
      ).catch(
        err => console.log('Fetch Error :-S', err)
      );
    })
  }

  window.addEventListener('DOMContentLoaded', () => {
    updateGpioStatuses()
  })
})()
