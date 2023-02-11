window.addEventListener('load', () => {

    const shower  = document.getElementById('show_pinout')
    const overlay = document.getElementById('overlay')

    shower.addEventListener('click', (event) => {
        event.preventDefault();
        event.stopPropagation();
        overlay.style.display = "grid";
    });

    overlay.addEventListener('click', (event) => {
        overlay.style.display = "none";
    });

});