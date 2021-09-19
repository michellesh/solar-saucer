const HEIGHT = 260;
const WIDTH = 260;

const SOLAR_PANEL_WIDTH = 45;
const SOLAR_PANEL_LENGTH = 65;

// The inner ring circumference is 408"
const INNER_CIRCUMFERENCE = 408;
const INNER_RADIUS = INNER_CIRCUMFERENCE / Math.PI / 2; // C=2πr

// The outer ring is 65" wider than the inside ring
const OUTER_RADIUS = INNER_RADIUS + SOLAR_PANEL_LENGTH;

const NUM_DOTS_OUTER = 120;
const NUM_DOTS_INNER = 80;
const NUM_STRANDS = 18;
const STRAND_LENGTH = 50;

const XY = (x, y) => Object.freeze({ x, y });

const Ring = (radius, origin) => Object.freeze({
  cx: origin.x,
  cy: origin.y,
  origin,
  r: radius,
  draw: ctx => {
    ctx.beginPath();
    ctx.arc(origin.x, origin.y, radius, 0, 2 * Math.PI);
    ctx.stroke();
  }
});

const Dot = (xy, color = 'black', radius = 2) => Object.freeze({
  x: xy.x,
  y: xy.y,
  draw: ctx => {
    ctx.beginPath();
    ctx.arc(xy.x, xy.y, radius, 0, 2 * Math.PI);
    ctx.fillStyle = color;
    ctx.fill();
  }
});

const Rectangle = (p1, p2, p3, p4) => Object.freeze({
  p1,
  p2,
  p3,
  p4,
  draw: ctx => {
    ctx.beginPath();
    ctx.moveTo(p1.x, p1.y);
    ctx.lineTo(p2.x, p2.y);
    ctx.lineTo(p3.x, p3.y);
    ctx.lineTo(p4.x, p4.y);
    ctx.fillStyle = 'rgba(0, 255, 255, 0.5)';
    ctx.fill();
  }
});

const Line = (p1, p2) => Object.freeze({
  p1,
  p2,
  draw: ctx => {
    ctx.beginPath();
    ctx.moveTo(p1.x, p1.y);
    ctx.lineTo(p2.x, p2.y);
    ctx.strokeStyle = 'black';
    ctx.stroke();
  },
  output: [
    '(',
    Math.round(p1.x),
    ',',
    Math.round(p1.y),
    ') -> (',
    Math.round(p2.x),
    ',',
    Math.round(p2.y),
    ')'
  ].join(' ')
});

const pointOnCircumference = (radius, origin, d) => {
  const degrees = d - 90 < 0 ? d + 270 : d - 90;
  const radians = (degrees * Math.PI) / 180;
  return XY(
    origin.x + radius * Math.cos(radians),
    origin.y + radius * Math.sin(radians)
  );
};

const pointOnLine = (p1, p2, howFar) => XY(
  p1.x + (p2.x - p1.x) * howFar,
  p1.y + (p2.y - p1.y) * howFar
);

const inner = Ring(INNER_RADIUS, XY(WIDTH / 2, HEIGHT / 2));
const outer = Ring(OUTER_RADIUS, XY(WIDTH / 2, HEIGHT / 2));

const outerCornerStep = 20;
const innerCornerStep = 40;

// Calculate the corners of the rectangles (solar panels)
const innerPanelCorners = d3.range(0, 360, innerCornerStep).map(degrees =>
  pointOnCircumference(inner.r, inner.origin, degrees)
);
const outerPanelCorners = d3.range(10, 360, outerCornerStep).map(degrees =>
  pointOnCircumference(outer.r, outer.origin, degrees)
);

// Group corners into rectangles (solar panels)
const solarPanels = d3.range(0, 9).map(i => Rectangle(
  innerPanelCorners[i],
  outerPanelCorners[i * 2],
  outerPanelCorners[(i * 2) + 1],
  innerPanelCorners[i + 1 >= 9 ? 0 : i + 1]
));

// Group rectangle edges into lines (led strips)
const ledStrips = solarPanels.reduce((acc, rectangle) => {
  acc.push(Line(rectangle.p1, rectangle.p2));
  acc.push(Line(rectangle.p4, rectangle.p3));
  return acc;
}, []);

const offsetStripNumber = i => i < 6 ? i + 12 : i - 6;

// Find the XY coords of an LED given the strip number and index
const getLedXY = (stripNumber, index) => 
  pointOnLine(
    ledStrips[offsetStripNumber(stripNumber)].p1,
    ledStrips[offsetStripNumber(stripNumber)].p2,
    index / STRAND_LENGTH
  );

// Outer dots
const outerStart = 10 + (12 * outerCornerStep);
const outerEnd = 10 + (5 * outerCornerStep);
const outerDotStep = outerEnd / (NUM_DOTS_OUTER / 2);

// Find the XY coords of an inner Dot LED given the index
const getOuterRingXY = index => {
  const degrees = index < (NUM_DOTS_OUTER / 2)
    ? (outerDotStep * index) + outerStart
    : outerDotStep * (index - NUM_DOTS_OUTER / 2);
  return pointOnCircumference(outer.r, outer.origin, degrees);
};

// Inner dots
const innerStart = 6 * innerCornerStep;
const innerDotStep = 360 / NUM_DOTS_INNER;

// Find the XY coords of an inner Dot LED given the index
const getInnerRingXY = index => {
  const degrees = index < (NUM_DOTS_INNER / 3)
    ? (innerDotStep * index) + innerStart
    : innerDotStep * (index - NUM_DOTS_INNER / 3);
  return pointOnCircumference(inner.r, inner.origin, degrees);
};

// Define the strand LEDs
const leds = d3.range(NUM_STRANDS).map(strand =>
  d3.range(STRAND_LENGTH).map(pixel => Dot(getLedXY(strand, pixel)))
);

// Define the outer dots
const outerDots =
  d3.range(outerStart, 360, outerDotStep).map(d =>
    Dot(pointOnCircumference(outer.r, outer.origin, d))
  ).concat(d3.range(0, outerEnd, outerDotStep).map(d =>
    Dot(pointOnCircumference(outer.r, outer.origin, d))
  ));

// Define the inner dots
const innerDots =
  d3.range(innerStart, 360, innerDotStep).map(d =>
    Dot(pointOnCircumference(inner.r, inner.origin, d))
  ).concat(d3.range(0, innerStart, innerDotStep).map(d =>
    Dot(pointOnCircumference(inner.r, inner.origin, d))
  ));

const draw = () => {
  const canvas = document.getElementById('canvas');
  if (canvas.getContext) {
    const ctx = canvas.getContext('2d');

    // Draw the inner and outer rings
    inner.draw(ctx);
    outer.draw(ctx);

    // Draw dots on the corners of each solar panel
    innerPanelCorners.forEach(xy => Dot(xy).draw(ctx));
    outerPanelCorners.forEach(xy => Dot(xy).draw(ctx));

    // Draw the solar panels as rectangles
    solarPanels.forEach(panel => panel.draw(ctx));

    // Draw each LED strip as a line
    ledStrips.forEach(ledStrip => ledStrip.draw(ctx));

    // Output coordinates for all corners
    document.getElementById('output').innerHTML = d3.range(0, NUM_STRANDS)
      .map(i => i + ': ' + ledStrips[offsetStripNumber(i)].output)
      .join('<br />');

    // Output coordinates for each LED on each strip
    document.getElementById('output').innerHTML = d3.range(0, NUM_STRANDS)
      .flatMap(s => ([
        'x' + s + ': ' + d3.range(0, STRAND_LENGTH).map(
          i => Math.round(getLedXY(s, i).x)).join(', '),
        'y' + s + ': ' + d3.range(0, STRAND_LENGTH).map(
          i => Math.round(getLedXY(s, i).y)).join(', ')
      ])).join('<br />');

    // Output coordinates for each LED on inner ring
    document.getElementById('output').innerHTML = [
      'innerX: ' + d3.range(0, NUM_DOTS_INNER).map(
        i => Math.round(getInnerRingXY(i).x)).join(', '),
      'innerY: ' + d3.range(0, NUM_DOTS_INNER).map(
        i => Math.round(getInnerRingXY(i).y)).join(', ')
    ].join('<br />');

    // Output coordinates for each LED on outer ring
    document.getElementById('output').innerHTML = [
      'outerX: ' + d3.range(0, NUM_DOTS_OUTER).map(
        i => Math.round(getOuterRingXY(i).x)).join(', '),
      'outerY: ' + d3.range(0, NUM_DOTS_OUTER).map(
        i => Math.round(getOuterRingXY(i).y)).join(', ')
    ].join('<br />');

    // Draw LEDs
    innerDots.forEach(dot => dot.draw(ctx));
    outerDots.forEach(dot => dot.draw(ctx));
    leds.flatMap(strand => strand).forEach(led => led.draw(ctx));

    // Test getLedXY, highlight a random LED in red
    Dot(getLedXY(17, 20), 'red').draw(ctx);

    // Test getOuterRingXY, highlight a random LED in red
    Dot(getOuterRingXY(101), 'red').draw(ctx);

    // Test getInnerRingXY, highlight a random LED in red
    Dot(getInnerRingXY(1), 'red').draw(ctx);
  }
};
