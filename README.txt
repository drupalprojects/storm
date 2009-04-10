/* $Id$ */

Documentation for STORM project management module.
- Project homepage: http://drupal.org/project/storm

---

STORM is a series of modules that provide a basic framework of a project management application in Drupal. All components (except Attributes) are nodes, so can be extended as follows:

- CCK module can add additional fields to the various content types
- Views module (support yet to implement) can build customised views
- Taxonomy module (in Drupal core) can be used to tag the contents of nodes
- Upload module (in Drupal core) can be used to attach files
- Comment module (in Drupal core) can be used to permit comments on each node

---

Features:

- Attributes: to manage the lists of values used in STORM, like: tasks status, countries, currencies and so on.

- Organizations: the companies or individual stakeholders of your projects

- Projects: your projects. Every project can have multiple tasks hierarchically nested to build a work breakdown structure.

- Tasks: the parts that compose a project

- Tickets: every ticket can be associated with an organization, project and task

- Timetrackings: where you can register your activities on an organization, project, task or ticket

- Expenses: where you can register the expenses of a specific project or of your own activity

- People: organizations contacts

- Permission control: a fine grained permission control permits to share the data with other users and organizations

- Reporting: a simple and themeable support for reporting (with optional multilingual support)

- Notes: a note can be associated to an organization, project and task

- Knowledge base: to collect your tips, tricks and important pieces of knowledge.