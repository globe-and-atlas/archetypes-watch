# Project Context

## Identity

- Name: `archetypes-watch`
- Profile: `workflow-python`
- Deploy: `local-only`
- Runtime: `pebble-c`

## Purpose

Offer 16 directly selectable visual clocks in one Pebble watchface. Type codes are design labels, not a personality assessment. Station House is the default; Margin Notes carries the optional “NOTICE THE GOOD” phrase.

## Constraints

- One PBW for Emery, Gabbro, Chalk, and Diorite.
- Large clock text and persistent shared settings. The battery indicator was removed by user request on 2026-10-23.
- Date convention: Off, ISO, US, and EU formats; Off is the default.
- No per-minute JavaScript or continuous sensor use.

## Out of Scope

- Personality diagnosis or automatic type inference.
- Location, network telemetry, and continuous animation.
- Store publication and physical-watch acceptance during initial development.
